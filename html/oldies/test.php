<?php


// Conectando, seleccionando la base de datos
$link = mysql_connect('localhost', 'suprema', 'suprema')
    or die('No se pudo conectar: ' . mysql_error());
echo 'Connected successfully';
mysql_select_db('Suprema') or die('No se pudo seleccionar la base de datos');

// Realizar una consulta MySQL
$query = 'select r.Id,u.Nombre,u.Departamento,d.Puerta,d.Ip,e.evento,r.time from Registro r join (select max(time) a from Registro group by Dispositivo) x on r.Time = x.a join Usuarios u on r.Usuario = u.Id join Dispositivos d on d.Id = r. Dispositivo join Eventos e on r.Evento = e.Id';
$result = mysql_query($query) or die('Consulta fallida: ' . mysql_error());

// Imprimir los resultados en HTML
echo "<table>\n";
echo "\t\t<td>|ID|</td>\n";
echo "\t\t<td>|Nro Usuario|</td>\n";
echo "\t\t<td>|Nombre|</td>\n";
echo "\t\t<td>|Departamento|</td>\n";
echo "\t\t<td>|Evento|</td>\n";
echo "\t\t<td>|Puerta|</td>\n";
echo "\t\t<td>|Ip|</td>\n";
echo "\t\t<td>|Time|</td>\n";
// Sacamos de a una las lineas de la consulta
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    echo "\t<tr>\n";
    foreach ($line as $col_value) {
        echo "\t\t<td>|$col_value|</td>\n";
    }
    echo "\t</tr>\n";
}
echo "</table>\n";

// Liberar resultados
mysql_free_result($result);

// Cerrar la conexión
mysql_close($link);

//Refresh de la pagina automatico
$url1=$_SERVER['REQUEST_URI'];
header("Refresh: 5; URL=$url1");
?>
