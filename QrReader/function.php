<?php
if (isset($_GET["data"]) && !empty($_GET["data"])) {


    $ret = exec('InOutQr.exe '.$_GET["data"],$output,$return_var);

    if ($return_var == 1) {
        echo "Bienvenue";
    }
    elseif ($return_var == 2) {
        echo "Au revoir";
    }
    else {
        echo "ERROR : Please contact an administrator";
    }
}
 ?>

 <br>
 <br>
 <form action="index.php">
<button>Retour</button>
 </form>
