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
  	console.log('Connection established');
	});

	var consulta = "Select Nombre from Usuarios where Id = " + Id;
	con.query(consulta,function(err,rows){
  	if(err) throw err;

  	console.log('Data received from Db:\n');
  	console.log(rows[0].Nombre);
	});


	con.end(function(err) {
  	// The connection is terminated gracefully
  	// Ensures all previously enqueued queries are still
  	// before sending a COM_QUIT packet to the MySQL server.
	});
}

Consulta("92");
