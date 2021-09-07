// web server in express
const express = require('express');
const fs = require('fs');
const app = express();
const port = 80;

const doorsFile = '/home/root/doors_status.txt';

app.use(express.json()); //Used to parse JSON bodies
app.use(express.static('dist'))


// get doors
app.get('/doors', (_req, res) => {
    fs.readFile(doorsFile, 'utf8', (err, data) => {
        if (err) throw err;
        res.send(data.trim().split(' '));

    });
});


app.get('/hello', (req, res) => {
    res.send('Hello World!');
})

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`);
})