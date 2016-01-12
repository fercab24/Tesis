#!/usr/bin/env node

var clientes = [];
var cliente;
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
        });

        var consulta = "Select Nombre from Usuarios where Id = " + Id;
        con.query(consulta,function(err,rows){
        if(err) throw err;

        console.log('Data received from Db:\n');
        console.log("Nombre = " + rows[0].Nombre);
        connection.send("Nombre = " + rows[0].Nombre);
        });


        con.end(function(err) {
        });
}

function mandar(usuario,dispositivo,evento,nombre,time){
	console.log("Usuario = ".concat(dispositivo));
        console.log("Evento = ".concat(evento));
        console.log("Time = ".concat(time));
        console.log("Nombre = " + nombre);
        for(cliente in clientes){
	        clientes[cliente].send("Dispositivo = " + dispositivo);
                clientes[cliente].send("Usuario = " + usuario);
                clientes[cliente].send("Evento = " + evento);
                clientes[cliente].send("Nombre = " + nombre);
	}
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
        	});

        	var consulta = "Select Nombre from Usuarios where Id = " + newRow.fields.Usuario;
		nombre = "Esto no va";
        	con.query(consulta,function(err,rows){
        	if(err) throw err;
        		//console.log('Data received from Db:\n');
			nombre = rows[0].Nombre; //Para que no sea el mismo objeto y no lo borre el con.end, duplicamos cadena
			mandar(newRow.fields.Usuario,newRow.fields.Dispositivo,newRow.fields.Evento,nombre,newRow.fields.Time);
        		//console.log("Nombre = " + nombre);
        		//connection.send("Nombre = " + rows[0].Nombre);
        	});


		//mandar(newRow.fields.Usuario,newRow.fields.Dispositivo,newRow.fields.Evento,nombre);
                /*console.log("Usuario = ".concat(newRow.fields.Usuario));
                console.log("Evento = ".concat(newRow.fields.Evento));
                console.log("Time = ".concat(newRow.fields.Time));
                console.log("Nombre = " + nombre);
		for(cliente in clientes){
                        clientes[cliente].send("Dispositivo = ".concat(newRow.fields.Dispositivo));
                        clientes[cliente].send("Usuario = ".concat(newRow.fields.Usuario));
                        clientes[cliente].send("Evento = ".concat(newRow.fields.Evento));
                        clientes[cliente].send("Nombre = " + nombre);
			

                }*/
		
        	con.end(function(err) {
        	});

	}
});



//Aceptamos conexiones 
wsServer.on('request', function(request) {
    if (!originIsAllowed(request.origin)) {
      // Make sure we only accept requests from an allowed origin 
      request.reject();
      console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
      return;
    }
    var connection = request.accept(null, request.origin); //Aca esta el moco, como es una variable global, solo guarda una conexion,
							//Solo tiene la conexion mas reciente, no importa cuantos clientes se conecten,
							//solo guarda la mas reciente, y se lo manda a ese nomas.
	
    clientes.push(connection);	
    console.log((new Date()) + ' Connection accepted.');
    var index = clientes.indexOf(connection);
    console.log("Agregado en la lista, posicion = " + index);


    connection.on('close', function(reasonCode, description) {
	var index = clientes.indexOf(connection);
	clientes.splice(index,1);
	//clientes.pop(connection);
	console.log("Estaba en la posicion " + index + " Nuevo tamaño " +clientes.length);
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
    });
});



