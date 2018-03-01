<?php
	include "header.php";
	require "functions.php";

	if( isset($_GET["register"]) && $_GET["register"] == "ok"){
		echo '<script>alert("Votre compte a bien été créé.");</script>';
	}

	if( isset($_GET["wtf"]) && $_GET["wtf"] == "true"){
		echo '<script>alert("Vous voulez peut-être que je vous apporte un café ? Avec ou sans sucre ?");</script>';
	}
?>

	<section>

		<h1>Bonjour ! Bienvenue sur le site de soutien de l'ESGI !</h1>

		<div class="col-xs-offset-3 col-xs-6 bg-info">

			<!-- PROGRESSION POUR LVL 2 -->
			<?php
				if( isset($_SESSION["level"]) && $_SESSION["level"] == 2 ){
			?>
					<h1>VOTRE PROGRESSION</h1>

					

			<?php
				$db = connectDb();

				// COMPTABILISATION DES EXERCICES
				$query = $db->prepare("SELECT COUNT(*) FROM LINK_LEARNING WHERE id_user=:id_user");
				$query->execute([ "id_user"=>$_SESSION["user_connected"] ]);
				$total = $query->fetch();

				// COMPTABILISATION DES EXERCICES TERMINES
				$query = $db->prepare("SELECT COUNT(*) FROM LINK_LEARNING WHERE id_user=:id_user AND learned=:learned");
				$query->execute([ 
							"id_user"=>$_SESSION["user_connected"],
							"learned"=>1
						]);
				$finish = $query->fetch();

				// CALCUL POUR BARRE DE PROGRESSION
				$progress = $finish[0] * 100 / $total[0];
				echo '
					<div class="progress">
						<div class="progress-bar progress-bar-striped active" role="progressbar"
					  	aria-valuenow="'.$progress.'" aria-valuemin="0" aria-valuemax="100" style="width:50%">'.$progress.'%
					  </div>
					</div>
					';



				echo 'Vous devez terminer au total '.$total[0].' exercices que vos responsables de soutien vous ont donné.<br>';

				echo 'Vous en avez terminé '.$finish[0].'.<br><br>';


			?>


				</div>

			<?php
				}
			?>
		</div>

	</section>





<?php
	include "footer.php";
?>