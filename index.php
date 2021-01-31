<?php

$sunucu = stream_socket_server("tcp://0.0.0.0:4444", $errno, $errorMessage);

while (1) 
{
    $msg = "";
    $hata =0;

    print("Listening.... \n");
	$istemci = stream_socket_accept($sunucu);

    if(!$istemci) continue;

    print "Metadata alınıyor...\n";
    $meta = getir_metadata($istemci);

    print_r($meta);

    $dosya = getir_dosyaVeri($istemci);


    if($meta["Page"] == "/upload"){
        
        print "İçerik alınıyor...\n";
        $icerik = getir_icerik($istemci,$meta);

        $fp = fopen($dosya["Content-Disposition"]["filename"],"w");
        fwrite($fp,$icerik);
        fclose($fp);
        
    }
  

    print("Sayfa oluşturuluyor.... \n");
    $Sdata = olustur_sayfa($msg);
    print("Sayfa gönderiliyor.... \n");
    stream_socket_sendto($istemci,$Sdata);

    print "Bağlantı sonlandırılıyor...\n";
    fclose($istemci);
}

fclose($server);

function getir_metadata($client){

    $Rdata = stream_get_line($client,9999999,"\r\n\r\n");
    $array = explode("\r\n",$Rdata);

    $head = explode(" ",$array[0]);
    $liste["Method"]=$head[0];
    $liste["Page"] = $head[1];
    $liste["Http"] =$head[2];

    array_splice($array,0,1);

    foreach($array as $part){
        $p = explode(":",$part);
        $liste[$p[0]] = $p[1];      
    }

    return $liste;
}

function getir_dosyaVeri($client){

    $Rdata = stream_get_line($client,9999999,"\r\n\r\n");
    $array = explode("\r\n",$Rdata);

    foreach($array as $part){
        $p = explode(":",$part);
        $liste[$p[0]] = $p[1];      
    }

    $liste["Content-Disposition"] = explode(";",$liste["Content-Disposition"]);

    foreach($liste["Content-Disposition"]  as $part){
        $p = explode("=",$part);
        $l[$p[0]] = $p[1];      
    }

    $liste["Content-Disposition"] = $l;

    print_r($liste);
    return $liste;

}


function getir_icerik($client,$meta){
    $len = $meta["Content-Length"];
    if($len == 0)
        return NULL;

    return stream_get_line($client,$len);
}



function olustur_sayfa($extra){

    ob_start();
    form();
    print $extra;
    $output = ob_get_contents();
    ob_end_clean();
    $header = "HTTP/1.0 200 OK\r\n";
    $header .= "Content-Type: text/html\r\n\r\n";
    $data = $header . $output;
    return $data;

}


function form(){ ?>
<meta charset="UTF-8">
<center>
<table border=0>
<form action="upload" method=post enctype="multipart/form-data">
<tr>
<td><input type='file' name='file' id='file' ></td>
<td><input type="submit" value="Yükle" name="submit" ></td>
</tr>
</form>
</table>
<?php }?>