<?php
// Crear una imagen en blanco y añadir algún texto
//$im = imagecreatetruecolor(1280, 1;
//$im = imagecreatefrompng("http://3.bp.blogspot.com/-Kxtl81f1CTU/UTYwllTZ6mI/AAAAAAAABf4/-XjW9hif_d0/s1600/Belgrano.png");
//include 'image.php';
$im = imagecreatefrompng("images/54902.png");


// Establecer la cabecera de tipo de contenido - en este caso image/jpeg
header('Content-Type: image/png');

// Imprimir la imagen
imagepng($im);

// Liberar memoria
imagedestroy($im);

//Refresh de la pagina automatico
$url1=$_SERVER['REQUEST_URI'];
header("Refresh: 2; URL=$url1");

?>
