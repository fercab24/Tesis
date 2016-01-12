<?php
function crear($fullname,$aparato,$evento)
{

	//Manipulamos para poner el nombre y debajo el apellido
	$fullname = explode(" ",$fullname);
	$string = $fullname[0]."\n".$fullname[1];
	//echo "va";
	//Elegimos el prediseño de la imagen
	if($evento == "3") {echo "entro"; $imagepath="images/ok.png";}
	//echo "salio";
	else if($evento == "4")$imagepath="images/NotOk.png";
	else{ 
		$imagepath = "images/warning.png";
		$string = "Usuario No\nRegistrado";
	}

	//$string = $nombre." ".$evento." ".$aparato;
	//echo $string;
	
	//Creamos la imagen y añadimos el texto
	$image1 = @imagecreatefrompng( $imagepath)
		or die('No se puede Iniciar el nuevo flujo a la imagen GD');
	$color_texto = imagecolorallocate($image1, 0, 0, 0);
	imagettftext($image1,25,0,60,60,$color_texto,"/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",$string);
	imagepng($image1,"images/".$aparato.".png");
	imagedestroy($image1);



}

crear("Juani Cardinalli","544953454","4");

?>
