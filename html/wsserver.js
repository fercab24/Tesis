#!/usr/bin/env node


//Definimos la funcion para la consulta en la base de datos
function Consulta(Id){
        var mysql = require("./node_modules/mysql");

        // First you need to create a connection to the db
        var con = mysql.createConnection({
                host: "localhost",
                user: "root",
                password: "root",
                database: "Suprema"
        });

        con.connect(function(err){
        if(err){
                console.log('Error connecting to Db');
        return;
        }
        //console.log('Connection established');
        });

        var consulta = "Select Nombre from Usuarios where Id = " + Id;
        con.query(consulta,function(err,rows){
        if(err) throw err;

        console.log('Data received from Db:\n');
        console.log("Nombre = " + rows[0].Nombre);
        connection.send("Nombre = " + rows[0].Nombre);
	//return rows[0].Nombre;
        });


        con.end(function(err) {
        // The connection is terminated gracefully
        // Ensures all previously enqueued queries are still
        // before sending a COM_QUIT packet to the MySQL server.
        });
}




//Creamos la conexion para observar eventos en la base de datos
MySQLEvents = require('./node_modules/mysql-events');
var dsn = {
  host:     'localhost',
  user:     'root',
  password: 'root',
};
//Iniciamos el proceso watcher para que observe cambios en la base de datos
var mysqlEventWatcher = MySQLEvents(dsn);


//Creamos el websocket servidor para poder servir los datos a las conexiones websockets html
var WebSocketServer = require('websocket').server;
var http = require('http');

//Iniciamos el Servidor de Websockets 
var server = http.createServer(function(request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});

//Quedamos a la espera de nuevos clientes
server.listen(8080, function() {
    console.log((new Date()) + ' Server is listening on port 8080');
});
 
wsServer = new WebSocketServer({
    httpServer: server,
    // You should not use autoAcceptConnections for production 
    // applications, as it defeats all standard cross-origin protection 
    // facilities built into the protocol and the browser.  You should 
    // *always* verify the connection's origin and decide whether or not 
    // to accept it. 
    autoAcceptConnections: false
});
 
function originIsAllowed(origin) {
  // put logic here to detect whether the specified origin is allowed. 
  return true;
}

//Abrimos el watcher
var watcher =mysqlEventWatcher.add(
        'Suprema',
        function (oldRow, newRow) {
        //row inserted 
        if (oldRow === null) {
                console.log("Usuario = ".concat(newRow.fields.Usuario));
                console.log("Evento = ".concat(newRow.fields.Evento));
                console.log("Time = ".concat(newRow.fields.Time));
		//Consulta(newRow.fields.Usuario);
		if(typeof connection === 'undefined' || connection === null){console.log("No Client Connected yet..."); }
		else{
		Consulta(newRow.fields.Usuario);
                connection.send("Dispositivo = ".concat(newRow.fields.Dispositivo));
                connection.send("Usuario = ".concat(newRow.fields.Usuario));
                connection.send("Evento = ".concat(newRow.fields.Evento));
		}}}
);



//Aceptamos conexiones 
wsServer.on('request', function(request) {
    if (!originIsAllowed(request.origin)) {
      // Make sure we only accept requests from an allowed origin 
      request.reject();
      console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
      return;
    }
    connection = request.accept(null, request.origin); //Aca esta el moco, como es una variable global, solo guarda una conexion,
							//Solo tiene la conexion mas reciente, no importa cuantos clientes se conecten,
							//solo guarda la mas reciente, y se lo manda a ese nomas.
    console.log((new Date()) + ' Connection accepted.');

    //Abrimos el watcher
    /*var watcher =mysqlEventWatcher.add(
	'Suprema',
  	function (oldRow, newRow) {
     	//row inserted 
    	if (oldRow === null) {		
        	console.log("Usuario = ".concat(newRow.fields.Usuario));
        	console.log("Evento = ".concat(newRow.fields.Evento));
        	console.log("Time = ".concat(newRow.fields.Time));
		//Consulta(newRow.fields.Usuario);
                connection.send("Dispositivo = ".concat(newRow.fields.Dispositivo));
                connection.send("Usuario = ".concat(newRow.fields.Usuario));
                connection.send("Evento = ".concat(newRow.fields.Evento));		
		}}
	);*/

    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
    });
});



