// Función para crear y agregar estilos CSS al documento
function addStyles() {
	const style = document.createElement("style");
	style.innerHTML = `
		.container {
	    position: fixed; /* Cambiar a fixed para que se superponga */
	    top: 50%; /* Centrar verticalmente */
	    left: 50%; /* Centrar horizontalmente */
	    transform: translate(-50%, -60%); /* Ajustar la posición para centrar */
	    width: 90%; /* Ancho de la barra de carga */
		}

		#status {
			color: var(--color-foreground);
	    text-align: center;
	    margin: 0.5em;
	    font-size: 18px;
		}

		.progress-container {
	    border-radius: 10px;
	    position: relative;
	    height: 2em;
		}

		.progress-background {
	    background-color: var(--color-foreground);
	    height: 100%;
	    border-radius: 10px; /* Bordes redondeados */
		}

		.progress-bar {
	    background-color: var(--color-secundary); /* Color de la barra de progreso */
	    height: 100%;
			padding: 1px;
	    transform: translate(-1px, -1px); /* Ajustar la posición para centrar */
	    border-radius: 10px; /* Bordes redondeados */
	    transition: width 0.3s ease;
	    position: absolute; /* Posicionamiento absoluto para superponerse al fondo */
	    top: 0; /* Alinear al borde superior */
	    left: 0; /* Alinear al borde izquierdo */
		}

		.progress-text {
	    position: absolute;
	    right: 2%;
	    top: -20%;
	    font-weight: bold;
		}

		/* Estilo para el fondo de la barra de carga */
		#progressbar {
	    position: fixed;
	    top: 50%;
	    left: 50%;
	    transform: translate(-50%, -50%); /* Ajustar la posición para centrar */
	    width: 40%;
	    height: 6.5em;
	    border-radius: 10px; /* Bordes redondeados */
			background-color: var(--color-hover);
	    z-index: 1000; /* Asegurarse de que esté por encima de los demás elementos */
		}
	`;
	document.head.appendChild(style); // Agregar el estilo al head del documento
}

function progressBar() {
	addStyles(); // Llamar a la función para agregar los estilos
	// Agregar el sidebar al DOM
	document.getElementById("progressbarjs").innerHTML += `
			<div id="progressbar">			
				<div class="container">
						<p id="status">Reiniciando...</p>
						<div class="progress-container">
							<div class="progress-background">
								<div class="progress-bar" id="progressBar"></div>
								<p class="progress-text" id="progressText">0%</p>
							</div>
						</div>
				</div>
			</div>
	`;
	
	let progress = 0;
	const progressBar = document.getElementById("progressBar");
	const progressText = document.getElementById("progressText");
	
	const interval = setInterval(() => {
		if (progress++ < 100) {
			progressBar.style.width = progress + "%";
			progressText.textContent = progress + "%";
		}
		else {
			clearInterval(interval);
			document.getElementById("status").textContent = "Cambios aplicados correctamente";
			setTimeout(() => { location.reload(); }, 2000); // Recarga la página después de 2 segundos
		}
	}, 100); // Cambia el valor para ajustar la velocidad de la carga
}