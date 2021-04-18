require('dotenv').config();

const Logger = require('js-logger');
Logger.useDefaults();

const Koa = require('koa');
const cors = require('kcors');

const app = new Koa();
const port = process.env.PORT || 5000;

const origin = process.env.CORS_ORIGIN || '*';
app.use(cors({ origin }));

app.use(async (ctx, next) => {
    const start = Date.now();
    await next();
    const responseTime = Date.now() - start;
    Logger.info(`${ctx.method} ${ctx.status} ${ctx.url} - ${responseTime} ms`);
});

app.use(async (ctx, next) => {
    try {
        await next();
    } catch (err) {
        ctx.status = err.status || 500;
        ctx.body = err.message;
        Logger.error(`Request error ${ctx.url} - ${err.message}`);
        Logger.debug(err.stack);
    }
});

const Postgres = require('pg');
const connectionString = process.env.DATABASE_URL;
const database = new Postgres.Client({ connectionString });

database.connect().then(() => {
    Logger.info(`Connected to ${database.database} at ${database.host}:${database.port}`);
}).catch(Logger.error);

const Router = require('koa-router');
const router = new Router();

router.get('/:z/:x/:y', async ctx => {
    // World constants for EPSG:3857
    const worldMercMax = 20037508.3427892;
    const worldMercMin = worldMercMax * -1;
    const worldMercSize = worldMercMax - worldMercMin;

    const {x, y, z} = ctx.params;
    Logger.info(`fetching tile at x:${x} y:${y} z:${z}`);

    const tileSize = Math.pow(2, z);
    Logger.info(`tilesize: ${tileSize}`);

    const tileMercSize = worldMercSize / tileSize;
    Logger.info(`projected tilesize: ${tileMercSize}`);

    xmin = worldMercMin + (tileMercSize * x);
    xmax = worldMercMin + (tileMercSize * (x + 1));
    ymin = worldMercMax - (tileMercSize * (y + 1));
    ymax = worldMercMax - (tileMercSize * y);

    const query =  `SELECT ST_AsMVT(q, 'poi') as tiles
                    FROM (
                        SELECT
                            gid, label,
                            ST_AsMVTGeom(
                                geom,
                                ST_Segmentize(ST_MakeEnvelope(${xmin}, ${ymin}, ${xmax}, ${ymax}, 3857), 2.388657),
                                4096,
                                256,
                                false
                            ) geom
                        FROM poi
                    ) q;`;
    Logger.info(`executing query: ${query}`);
    const result = await database.query(query);
    ctx.body = result.rows[0].tiles;
    ctx.header['content-type'] = 'application/x-protobuf';
    ctx.status = 200;
});

app.use(router.routes(), router.allowedMethods());

app.listen(port, () => { Logger.info(`Server listening on ${port}`)});
