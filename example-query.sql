SELECT ST_AsMVT(q, 'poi')
FROM (
    SELECT
        gid, label,
        ST_AsMVTGeom(
            geom,
            ST_Segmentize(ST_MakeEnvelope(9.554629, 0.000000, 19.109257, 9.554629, 3857), 2.388657),
            4096,
            256,
            false
        ) geom
    FROM poi
) q;