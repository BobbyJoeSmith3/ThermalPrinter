const SerialPort = require('serialport');

// list serial ports:
SerialPort.list(function (err, ports) {
  ports.forEach(function(port) {
    console.log(port.comName);
  });
});
