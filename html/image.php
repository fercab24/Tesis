<?php
function crear($nombre,$evento,$aparato)
{
	// Conectando, seleccionando la base de datos
	$link = mysql_connect('localhost', 'suprema', 'suprema')
	    or die('No se pudo conectar: ' . mysql_error());
	mysql_select_db('Suprema') or die('No se pudo seleccionar la base de datos');

	// Realizar una consulta MySQL
	$query = 'select u.Nombre as nombre, r.Evento as evento, r.Dispositivo as dispositivo from Registro r join (select max(time) a from Registro group by Dispositivo) x on r.Time = x.a join Usuarios u on r.Usuario = u.Id join Dispositivos d on d.Id = r. Dispositivo join Eventos e on r.Evento = e.Id';
	$result = mysql_query($query) or die('Consulta fallida: ' . mysql_error());

	// Sacamos de a una las lineas de la consulta
	while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
		$fullname = $line["nombre"];
    		$evento = $line["evento"];
    		$aparato = $line["dispositivo"];

		//Manipulamos para poner el nombre y debajo el apellido
		$fullname = explode(" ",$fullname);
		$string = $fullname[0]."\n".$fullname[1];

    		//Elegimos el prediseño de la imagen
    		if($evento == "3") $imagepath="images/ok.png";
    		else if($evento == "4")$imagepath="images/NotOk.png";
		else{ 
			$imagepath = "images/warning.png";
			$string = "Usuario No\nRegistrado";
		}

   		//$string = $nombre." ".$evento." ".$aparato;
		echo $string;
	
		//Creamos la imagen y añadimos el texto
		$image1 = @imagecreatefrompng( $imagepath)
      			or die('No se puede Iniciar el nuevo flujo a la imagen GD');
		$color_texto = imagecolorallocate($image1, 0, 0, 0);
		imagettftext($image1,25,0,60,60,$color_texto,"/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",$string);
		imagepng($image1,"images/".$aparato.".png");
		imagedestroy($image1);

	}

	// Liberar resultados
	mysql_free_result($result);

	// Cerrar la conexión
	mysql_close($link);
}
crear();
//Refresh de la pagina automatico
//$url1=$_SERVER['REQUEST_URI'];
//header("Refresh: 1; URL=$url1");

?>
