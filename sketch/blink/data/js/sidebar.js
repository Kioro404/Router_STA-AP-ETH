// Función para crear y agregar estilos CSS al documento
(function style() {
	const style = document.createElement("style");
	style.innerHTML = `
			/* The button used to open the list */
			.hamburger {
					position: fixed;
					right: 0em;
					/* transition: margin-right .5s; If you want a transition effect */
					padding: 0.25em;
					/* font-size: 1.3em; */
					cursor: pointer;
					background-color: var(--color-secundary);
					color: white;
					border: none;
					transition: 0.3s;
			}

			.hamburger img {
					width: 3em; /* Ajusta el tamaño de la imagen */
					height: 3em;
			}

			.hamburger:hover {
					background-color: var(--color-hover);
			}

			.barMenu {
					width: 80%; /* Ancho del menú */
					padding: 1em;
			}

			.menu {
					list-style-type: none;
					margin: 0;
					padding: 0;
					background-color: var(--color-secundary);
			}

			.elementMenu {
					position: relative;
			}

			.elementMenu a {
					display: block;
					color: var(--color-foreground);
					text-decoration: none;
					padding: 14px 16px;
			}

			.elementMenu a:hover {
					background-color: var(--color-hover);
			}

			/* Estilos del submenú */
			.subMenu {
					display: none; /* Oculto por defecto */
					list-style-type: none;
					margin: 0px;
					padding-left: 5px;
					background-color: var(--color-hover);
			}

			.subMenu a:hover {
					background-color: var(--color-secundary);
			}

			.subMENUsub a:hover {
					background-color: var(--color-hover);
			}
					
			/* The list menu */
			.list {
				height: 100%; /* 100% Full-height */
				width: 0; /* 0 width - change this with JavaScript */
				position: fixed; /* Stay in place */
				z-index: 1; /* Stay on top */
				top: 0;
				right: 0;
				background-color: var(--color-primary); /* Black*/
				overflow-x: hidden; /* Disable horizontal scroll */
				padding-top: 0.5em; /* Place content 60px from the top */
				transition: 0.3s; /* 0.5 second transition effect to slide in the list */
			}

			/* On smaller screens, where height is less than 450px, change the style of the sidenav (less padding and a smaller font size) */
			@media screen and (max-height: 450px) {
				.list {padding-top: 15px;}
				.list a {font-size: 18px;}
			}
	`;
	document.head.appendChild(style); // Agregar el estilo al head del documento
})();

// Agregar el sidebar al DOM
document.getElementById("sidebarjs").innerHTML += `
    <div id="sidebar">
        <button id="hamburger" class="hamburger">
            <img src="../assets/icons/menu.svg" alt="Menu">
        </button>
        <div id="list" class="list">
            <nav class="barMenu">
                <ul class="menu">
                    <li class="elementMenu">
                        <a href="../pages/index.html">Home</a>
                        <a href="#" class="no-link">Configuración</a>
                        <ul class="subMenu">
                            <li>
                                <a href="../pages/network.html">Redes</a>
                            </li>
                        </ul>
                    </li>
                </ul>
            </nav>
        </div>
    </div>
`;

(function sidebar() {
	const list = document.getElementById("list");
	const hamburger = document.getElementById("hamburger");

	var MAXwidth = "250px", MINwidth = "0";
	
	// Agrega el evento de clic al botón de hamburguesa
	hamburger.addEventListener("click", function() {
		// Verifica si el sidebar está abierto
		if (list.style.width === MAXwidth) closeNav(); // Cierra el sidebar
		else openNav(); // Abre el sidebar
	});

	// Cierra la barra lateral si se hace clic fuera de ella
	document.addEventListener("click", function(event) {
		const isClickInsideSidebar = list.contains(event.target);
		const isClickOnHamburger = hamburger.contains(event.target);

		if (!isClickInsideSidebar && !isClickOnHamburger && list.style.width === MAXwidth) closeNav();
	});

	/* Set the width of the list to 250px and the right margin of the page content to 250px */
	function openNav() {
		list.style.width = MAXwidth;
		hamburger.style.marginRight = MAXwidth;
	}

	/* Set the width of the list to 0 and the right margin of the page content to 0 */
	function closeNav() {
		list.style.width = MINwidth;
		hamburger.style.marginRight = MINwidth;
	}
})();

(function menubar() {
	// Selecciona todos los enlaces del menú principal
	const noLinks = document.querySelectorAll(".no-link");

	// Agrega un evento de clic a cada enlace del menú principal
	noLinks.forEach(noLink => {
		noLink.addEventListener("click", function(event) {
			event.preventDefault(); // Evita el comportamiento por defecto del enlace

			closeSubMenu(document.querySelectorAll(".elementMenu .subMenu"), this.nextElementSibling);
			// closeSubMenu(document.querySelectorAll(".elementMenu .subMenu .subMENUsub"), this.nextElementSibling);
		});
	});

	// Cierra otros submenús
	function closeSubMenu(subMenus, esta) {
		subMenus.forEach(subMenu => {
			if (subMenu !== esta) subMenu.style.display = "none";
		});

		// Alterna el submenú correspondiente
		if (esta) esta.style.display = esta.style.display === "block" ? "none" : "block";
	}
})();