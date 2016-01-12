MySQLEvents = require('./node_modules/mysql-events');
var dsn = {
  host:     'localhost',
  user:     'root',
  password: 'root',
};
var mysqlEventWatcher = MySQLEvents(dsn);
console.log("Ya nos conectamos, iniciando watcher");
var watcher =mysqlEventWatcher.add(
  'Suprema',
  function (oldRow, newRow) {
     //row inserted 
    if (oldRow === null) {
	console.log("Nuevo Registro Insertado");
	console.log("Base de Datos = ".concat(newRow.database));
	console.log("Tabla = ".concat(newRow.table));
	console.log("Id = ".concat(newRow.fields.Id));
	console.log("Dispositivo = ".concat(newRow.fields.Dispositivo));
	console.log("Usuario = ".concat(newRow.fields.Usuario));
	console.log("Evento = ".concat(newRow.fields.Evento));
	console.log("Time = ".concat(newRow.fields.Time));
     
     
    }
 
     //row deleted 
    if (newRow === null) {
      //delete code goes here 
    }
 
     //row updated 
    if (oldRow !== null && newRow !== null) {
      //update code goes here 
    }
  }
);
