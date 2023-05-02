<!DOCTYPE html>
<html>
<head>
	<title>Page des fichiers uploadés</title>
    <style>
        body {
            background-color: #fa94b6;
            margin: 0;
            padding: 0;
        }

        header {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            z-index: 999;
            background-color: #fa94b6;
            text-align: center;
            font-size: 2em;
            font-weight: bold;
            box-shadow: 0px 2px 5px rgba(255, 255, 255, 0.3);
        }
		
		header a {
			color: white;
			width: 100%;
			height: 100%;
			padding: 10px 0;
			display: block;
			text-decoration: none;
		}

        #menu {
            margin: 15px auto 0;
            max-width: 300px;
            text-align: center;
            padding: 0;
            list-style-type: none;
            /* border-top: 1px solid #ccc; */
        }

        #menu li {
            margin: 0px 0;
        }

		#menu a {
			display: block;
			color: white;
			font-weight: bold;
			text-align: center;
			/* padding: 14px 16px; */
			text-decoration: none;
			transition: background-color 0.3s ease;
			width: 100%;
			height: 50px;
			line-height: 50px;
            padding: 0;
		}


		#menu li:first-child a {
			background-color: rgb(80, 98, 253);
		}
		
		#menu li:nth-child(2) a {
			background-color: rgb(252, 215, 54);
		}
		
		#menu li:nth-child(3) a {
			background-color: rgb(59, 180, 59);
		}
		
		#menu li:nth-child(4) a {
			background-color: rgb(129, 70, 165);
		}
		#menu li:nth-child(5) a {
			background-color: rgb(3, 3, 3);
		}

        #menu a:hover,
		#menu li a:hover 
		{
			background-color: orange;
			box-shadow: 0 5px 5px #22222214;
			transform: scale(1.1);
        }


        footer {
            position: fixed;
            bottom: 0;
            left: 0;
            right: 0;
            background-color: #111;
            color: white;
            text-align: center;
            padding: 10px 0;
            font-size: 0.8em;
        }

        main {
			margin-top: 62px;
            padding-bottom: 60px;
        }

		img {
			width: 20%;
			height: 20%;
			display: block;
			margin: 10% auto 0; /* centrage de l'image */
			animation: rotate 4s linear infinite; /* animation rotation */
			transform-origin: center;
		}

		img:hover {
			display: block;
			animation-play-state: paused; /* pause de l'animation au survol */
			 /* transform: scale(1.5); agrandissement de l'image au survol */
			/* -webkit-transform:scale(1.5);  */
		}

        @keyframes rotate { /* définition de l'animation rotation */
            from {
                transform: rotate(0deg) scale(1);
            }
			25%{
                transform: rotate(90deg) scale(1.5);
			}
			50%{
                transform: rotate(180deg) scale(1);
			}
			75%{
                transform: rotate(270deg) scale(1.5);
			}
            to {
                transform: rotate(360deg) scale(1);
            }
        }
        @keyframes rotate_full_speed { /* définition de l'animation rotation */
            from {
                transform: rotate(0deg) scale(1);
            }
            to {
                transform: rotate(4800deg) scale(0);
            }
        }

    </style>
</head>
<body>
	<header>
		<a href="/index.html">WebServ</a>
	</header>
	<h1>Liste des fichiers uploadés</h1>
	<div id="files">
	<?php
	if (!is_dir('./upload')) {
		echo "<p>Le répertoire upload n'existe pas.</p>";
	} else {
		$files = glob('./upload/*');
		if (count($files) == 0) {
			echo "<p>Le répertoire upload est vide.</p>";
		} else {
			foreach ($files as $file) {
				echo "<div style='display: flex; align-items: center;'>";
				echo "<p style='flex: 1; margin-left: 10px; font-weight: bold;'>" . basename($file) . "</p>";
				echo "<p style='flex: 11; margin-left: 5px;'> - </p>";
				echo "<form style='display: inline-block;' action='del.php' method='POST'>";
				echo "<input type='hidden' name='file' value='" . urlencode(basename($file)) . "'>";
				echo "<button type='submit' name='delete' value='delete' style='flex: 1; color: red;'>Supprimer</button>";
				echo "</form>";
				echo "<a href='upload/" . urlencode(basename($file)) . "' download><button style='flex: 11; margin-left: 10px; color: green;'>Télécharger</button></a>";
				echo "</div>";
			}
		}
	?>
	</div>
	<hr>
	<h2>Envoyer un fichier</h2>
	<form action="up.php" method="POST" enctype="multipart/form-data">
		<input type="file" name="file">
		<input type="submit" value="Envoyer">
	</form>
	<?php } ?>
    <footer>Site web créé par Nicolas, Mathieu et Cyril</footer>
	<script>
		let sheep = document.querySelector('#sheep')
		document.querySelector('#sheep').addEventListener("click", ()=>{
			sheep.style = "animation: rotate_full_speed 4s linear;"
			setTimeout(() => {
				sheep.style = "display: none;"
			}, 4000);
			console.log("⠀ ⠀⠀⠀⠀⠀⠀⢀⡔⠋⢉⠩⡉⠛⠛⠛⠉⣉⣉⠒⠒⡦⣄⠀⠀⠀⠀⠀⠀⠀", '\n',
			"⠀⠀⠀⠀⠀⠀⢀⠎⠀⠀⠠⢃⣉⣀⡀⠂⠀⠀⠄⠀⠀⠀⠀⢱⠀⠀⠀⠀⠀⠀", '\n',
			"⠀⠀⠀⠀⠀⡰⠟⣀⢀⣒⠐⠛⡛⠳⢭⠆⠀⠤⡶⠿⠛⠂⠀⢈⠳⡀⠀⠀⠀⠀", '\n',
			"⠀⠀⠀⠀⢸⢈⢘⢠⡶⢬⣉⠉⠀⠀⡤⠄⠀⠀⠣⣄⠐⠚⣍⠁⢘⡇⠀⠀⠀⠀", '\n',
			"⠀⠀⠀⠀⠈⢫⡊⠀⠹⡦⢼⣍⠓⢲⠥⢍⣁⣒⣊⣀⡬⢴⢿⠈⡜⠀⠀⠀⠀⠀", '\n',
			"⠀⠀⠀⠀⠀⠀⠹⡄⠀⠘⢾⡉⠙⡿⠶⢤⣷⣤⣧⣤⣷⣾⣿⠀⡇⠀⠀⠀⠀⠀", '\n',
			"⠀⠀⠀⠀⠀⠀⠀⠘⠦⡠⢀⠍⡒⠧⢄⣀⣁⣀⣏⣽⣹⠽⠊⠀⡇⠀⠀⠀⠀⠀", '\n',
			"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⠪⢔⡁⠦⠀⢀⡤⠤⠤⠄⠀⠠⠀⡇");
		})
		</script>
</body>
</html>
