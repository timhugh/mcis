WITH
bounds AS (
    SELECT ST_Segmentize(ST_MakeEnvelope(0.000000, -38.218514, 38.218514, 0.000000, 3857), 9.554629) AS geom,
    ST_Segmentize(ST_MakeEnvelope(0.000000, -38.218514, 38.218514, 0.000000, 3857), 9.554629)::box2d as b2d
),
mvtgeom AS (
    SELECT ST_AsMVTGeom(ST_Transform(t.geom, 3857), bounds.b2d) AS geom,
        label
    FROM land t, bounds
    WHERE ST_Intersects(t.geom, ST_Transform(bounds.geom, 26918))
)
SELECT ST_AsMVT(mvtgeom.*) FROM mvtgeom;


SELECT ST_AsMVT(q, 'pois', 4096, 'geom')
FROM (
    SELECT
        label,
        ST_AsMVTGeom(
            geom,
            ST_MakeEnvelope
        )
)