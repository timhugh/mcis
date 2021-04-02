
const VectorTile = require('@mapbox/vector-tile').VectorTile;
const Protobuf = require('pbf');
const axios = require('axios');

const args = process.argv.slice(2);
if (args.length != 1) {
    throw("Pass tile URL as an argument");
}
const tilePath = args[0];

console.log('Fetching tile from', tilePath);

const dataToTile = (data) => {
    console.log('data', data);
    const pbf = new Protobuf(data);
    console.log('pbf', pbf);
    const vt = new VectorTile(pbf);
    console.log('vt', vt);
};

axios({
    method: 'get',
    url: tilePath,
    responseType: 'arraybuffer',
    headers: {
        "Accept": "application/x-protobuf",
    },
})
    .then((response) => {
        console.log("response headers", response.headers);
        return dataToTile(response.data)
    })
    .catch((error) => console.log("[ERROR]", error));
