var SerialPort = require('serialport');
var WebSocketServer = require('ws').Server;
const Printer = require('thermalprinter');
const gm = require('gm');
const request = require('request');

var portName = "/dev/tty.usbmodemFD131"; // get port name from the command line
var myPort = new SerialPort(portName, {
  baudRate: 19200,
  options: false
});

const PRINT_WIDTH = 384; // Max print width of thermal printer

var Readline = SerialPort.parsers.Readline; // make instance of Readline parser
var parser = new Readline("\r\n"); // make a new parser object to read ASCII lines
myPort.pipe(parser); // pipe the serial stream to the parser

myPort.on('open', showPortOpen);
myPort.on('close', showPortClosed);
myPort.on('error', showError);
parser.on('data', readSerialData); // using parser object instead of serialport object



// Callback functions
function showPortOpen() {
  console.log('Port open. Data rate: ' + myPort.baudRate);
}
function showPortClosed() {
  console.log('port closed.');
}
function showError() {
  console.log('Serial port error: ' + error);
}
function readSerialData(data) {
  console.log(`data: ${data}`);
  // Send the latest data to all available webSocket clients
  if (connections.length > 0) { // if there are webSocket connections
    broadcast(data); // send the serial data to all of them
  }
  /*
    d = start speech
    w = pause speech
    a = restart
    s = print
  */
  if (data == "D\r") {
    console.log("Start Speech Recording");
  }
  if (data == "W\r") {
    console.log("Pause Speech Recording");
  }
  if (data == "A\r") {
    console.log("Restart");
  }
  if (data == "S\r") {
    console.log("Print");
    // broadcast(sayHello());
  }

  // if (data == "X\r") {
  //   console.log(`Received Print Button Data ${data}`);
  //   var aMessage = "Fires can't be made with dead embers, nor can enthusiasm be stirred by spiritless men. Enthusiasim in our daily work lightens effort and turns even labor into pleasant tasks\n";
  //   var splitMessage = aMessage.split("");
  //   for (character in splitMessage) {
  //     sendToSerial(splitMessage[character]);
  //     sleep(10);
  //   }
  // } else {
  //   console.log(typeof(data));
  //   console.log(typeof("X"));
  //   console.log(data == "X\r");
  //   console.log(data == "X\n");
  //   console.log(data == "X\n\r");
  //   console.log(data == "\u0058\u000D\u000A");
  // }

}


// Function to send webSocket data to the serialport:
function sendToSerial(data) {
  console.log("Sending to serial: " + data);
  myPort.write(data);
}


// create instance of WebSocketServer
var SERVER_PORT = 8081; // port number for the webSocket Server
var wss = new WebSocketServer({port: SERVER_PORT}); // the webSocket Server
var connections = new Array; // list of connections to the server

// webSocket event listeners
wss.on('connection', handleConnection);

function handleConnection(client) {
  console.log("New Connection"); // you have a new client
  connections.push(client); // add this client to the connections array

  client.on('message', sendToSerial); // when a client sends a message
  client.on('close', function() { // when a client closes its connection
    console.log("Connection Closed"); // print it out
    var position = connections.indexOf(client); // get the client's position in Array
    connections.splice(position, 1); // and delete it from array
  });
}

// Function to send serial data to the webSocket clients:
function broadcast(data) {
  for (myConnection in connections) { // iterate over the array of connections
    connections[myConnection].send(data); // send data to each connection
  }
}

function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}












function printImageAttempt() {
  /*
    maxPrintingDots = 0-255. Max heat dots, Unit (8dots), Default: 7 (64 dots)
    heatingTime = 3-255. Heating time, Unit (10us), Default: 80 (800us)
    heatingInterval = 0-255. Heating interval, Unit (10µs), Default: 2 (20µs)
    The more max heating dots, the more peak current will cost when printing,
    the faster printing speed. The max heating dots is 8*(n+1).
    The more heating time, the more density, but the slower printing speed.
    If heating time is too short, blank page may occur.
    The more heating interval, the more clear, but the slower printing speed.
  */

  var myPrinter = new Printer(myPort, {
    maxPrintingDots: 7,
    heatingTime: 80,
    heatingInterval: 2,
    commandDelay: 2
  });

  myPrinter.on('ready', function() {
    console.log('Printer ready!');
    sendToSerial('Print this!');

    getAndDitherImage(images)
      .then(resizedImgPath => {
        // printer.printImage(resizedImgPath).lineFeed(2).print(_ => console.log('Printed Image'));
        myPrinter
        .indent(10)
        .horizontalLine(16)
        .bold(true)
        .printLine('first line')
        .printImage(resizedImgPath)
        .print(function() {
          console.log('done');
          //process.exit();
        });
      })
      .catch(error => {
        console.error(error);
      });
  });

}

const getAndDitherImage = (imageArray) => new Promise((resolve, reject) => {
  console.log('Dithering image');

  if (imageArray.length < 1) {
    return reject('No image');
  }
  let imgFilename = imageArray.pop();
  let imgPath = `${__dirname}/portraits/${imgFilename}.jpg`;
  let resizedImgPath = `${__dirname}/resized/${imgFilename}.jpg`;
  console.log(resizedImgPath);

  gm(imgPath)
    .resize(PRINT_WIDTH, PRINT_WIDTH)
    .sharpen(5)
    .monochrome()
    .dither()
    .gravity('center')
    .extent(PRINT_WIDTH, PRINT_WIDTH)
    .autoOrient()
    .write(resizedImgPath, error => {
      if (error) {
        return reject(error);
      }
      resolve(resizedImgPath);
    });
});
