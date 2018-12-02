var SerialPort = require('serialport');
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
  console.log(data);
  // Send the latest data to all available webSocket clients
  // if (connections.length > 0) { // if there are webSocket connections
  //   broadcast(data); // send the serial data to all of them
  // }
  if (data) {
    console.log(`Received Print Button Data ${data}`);
    // myPort.write("Hello Bobby Joe");
    sendToSerial("Written to Serial. How long of a senetence can I write? What will this look like on the printer? I hope it doesn't continue to print strangely.");
  }
}


// Function to send webSocket data to the serialport:
function sendToSerial(data) {
  console.log("Sending to serial: " + data);
  myPort.write(data);
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
