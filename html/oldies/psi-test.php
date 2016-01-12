<?php
// Conectando, seleccionando la base de datos
$link = mysql_connect('localhost', 'suprema', 'suprema')
    or die('No se pudo conectar: ' . mysql_error());
mysql_select_db('Suprema') or die('No se pudo seleccionar la base de datos');

// Realizar una consulta MySQL
$query = 'select u.Nombre, r.Evento, r.Dispositivo from Registro r join (select max(time) a from Registro group by Dispositivo) x on r.Time = x.a join Usuarios u on r.Usuario = u.Id join Dispositivos d on d.Id = r. Dispositivo join Eventos e on r.Evento = e.Id';
$result = mysql_query($query) or die('Consulta fallida: ' . mysql_error());
$i = 0;
// Sacamos de a una las lineas de la consulta
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    echo $i;
    foreach ($line as $col_value) {
        echo "\t\t<td>|$col_value|</td>\n";
	//$string = $col_value;
        //echo "\t\t<td>$string</td>\n";
    }
    echo "\n";
    $i = $i + 1;
}

//echo "$string";

// Liberar resultados
mysql_free_result($result);

// Cerrar la conexión
mysql_close($link);

//header ('Content-Type: image/png');
$image1 = @imagecreatetruecolor(400, 400)
      or die('No se puede Iniciar el nuevo flujo a la imagen GD');
$color_texto = imagecolorallocate($image1, 233, 14, 91);
//$string = "Te fuiste al argentino A por puto y cagon";
imagestring($image1, 10, 5, 10,  $string, $color_texto);
//imagepng($image1);
imagepng($image1,"../images/generada.png");
imagedestroy($image1);


?>
