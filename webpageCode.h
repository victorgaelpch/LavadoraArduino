const char webpageCode[]= R"=====(<!DOCTYPE html>
<html lang="en">
  <head>
    <title>Control lavadora</title>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <style>
      body {
        background-color: #0e2954;
      }
      h1,
      h2,
      h3 {
        background-color: #1f6e8c;
        color: white;
        font-family: "Lato", sans-serif;
        max-width: 700px;
        margin: 5px auto;
        padding: 10px;
        border-radius: 4px;
      }
      h4 {
        background-color: #84a7a1;
        font-family: "Lato", sans-serif;
        color: rgb(0, 0, 0);
        padding: 5px;
        max-width: 500px;
        border-radius: 4px;
        font-size: 16px;
      }
      label {
        background-color: #952e99;
        font-family: "Lato", sans-serif;
        color: white;
        padding: 10px;
        border-radius: 10px;
      }
      #rangevalue {
        font-size: 5vw;
        color: white;
      }
      input[type="range"] {
        /* Establecer el tamaño de la barra y otros estilos aquí */
        width: 300px;
        height: 10px;
        background-color: #ccc;
        border-radius: 5px;
      }
      .alto,
      .medio,
      .bajo {
        font-family: "Lato", sans-serif;
      }
      .alto-auto,
      .medio-auto,
      .bajo-auto {
        font-family: "Lato", sans-serif;
      }
      .automatico {
        background-color: #0b666a; /* Green */
        border: none;
        color: rgb(255, 255, 255);
        padding: 15px 32px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 16px;
      }

      .container {
        max-width: 500px;
        margin: 5px auto;
        padding: 20px;
        background-color: #0e2954;
      }

      h6 {
        text-align: center;
        margin-bottom: 10px;
      }

      .options {
        display: flexbox;
        justify-content: center;
      }

      .option {
        display: flex;
        align-items: center;
        cursor: pointer;
        user-select: none;
      }

      .checkbox {
        display: none;
      }

      .slider {
        width: 40px;
        height: 20px;
        background-color: #ccc;
        border-radius: 20px;
        margin-right: 10px;
        position: relative;
      }

      .slider:before {
        content: "";
        position: absolute;
        width: 18px;
        height: 18px;
        border-radius: 50%;
        background-color: #fff;
        top: 1px;
        left: 1px;
        transition: 0.2s;
      }

      .checkbox:checked + .slider {
        background-color: #51ff00;
      }

      .checkbox:checked + .slider:before {
        transform: translateX(20px);
      }

      span {
        font-size: 18px;
      }

      button {
        display: block;
        margin: 20px auto;
        padding: 10px 20px;
        font-size: 18px;
        cursor: pointer;
        background-color: #2196f3;
        color: #fff;
        border: none;
        border-radius: 4px;
      }

      button:hover {
        background-color: #001c30;
      }
    </style>
  </head>
  <body>
    <center>
      <h1>Modo automatico</h1>
      <label for="nivelAgua"
        >Selecciona el nivel de agua (Auto Y Manual):</label
      >
      <input
        type="range"
        value="3"
        min="1"
        max="3"
        id="rangeNivel"
        oninput="updateRangeValue()"
      />
      <output id="rangevalue" class="alto">Alto</output><br /><br />

      <button
        id="iniciar"
        onclick="consultaGET('automatico')"
        class="automatico"
      >
        Iniciar
      </button>

      <h1>Modo manual</h1>

      <h4>Selecciona los modos que quieras usar</h4>
      <div class="container">
        <div class="options">
          <label class="option">
            <input type="checkbox" class="checkbox" id="LLenar" />
            <div class="slider"></div>
            <span>LLenar</span>
          </label>
          <label class="option">
            <input type="checkbox" class="checkbox" id="Remojar" />
            <div class="slider"></div>
            <span>Remojar</span>
          </label>
          <label class="option">
            <input type="checkbox" class="checkbox" id="Lavar" />
            <div class="slider"></div>
            <span>Lavar</span>
          </label>
          <label class="option">
            <input type="checkbox" class="checkbox" id="Enjuagar" />
            <div class="slider"></div>
            <span>Enjuagar</span>
          </label>
          <label class="option">
            <input type="checkbox" class="checkbox" id="Exprimir" />
            <div class="slider"></div>
            <span>Exprimir</span>
            <label class="option">
              <input type="checkbox" class="checkbox" id="Centrifugado extra" />
              <div class="slider"></div>
              <span>Centrifugado extra</span>
            </label>
          </label>
        </div>
        <button id="iniciar" onclick="iniciarModo()" class="automatico">
          Iniciar
        </button>
        <h4>Controles de mando</h4>
        <button id="iniciar" onclick="consultaGET('pausar')" class="automatico">
          Pausar
        </button>
        <button
          id="iniciar"
          onclick="consultaGET('reanudar')"
          class="automatico"
        >
          Reanudar
        </button>
        <button
          id="iniciar"
          onclick="consultaGET('reiniciar')"
          class="automatico"
        >
          Reiniciar
        </button>
      </div>
    </center>
    <script>
      function updateRangeValue() {
        var rangeValue = document.getElementById("rangeNivel").value;
        var rangeOutput = document.getElementById("rangevalue");
        switch (rangeValue) {
          case "1":
            rangeOutput.textContent = "Bajo";
            rangeOutput.className = "bajo";
            consultaGET(rangeOutput.className);
            break;
          case "2":
            rangeOutput.textContent = "Medio";
            rangeOutput.className = "medio";
            consultaGET(rangeOutput.className);
            break;
          case "3":
            rangeOutput.textContent = "Alto";
            rangeOutput.className = "alto";
            consultaGET(rangeOutput.className);
            break;
          default:
            rangeOutput.textContent = "Bajo";
            rangeOutput.className = "bajo";
            consultaGET(rangeOutput.className);
        }
      }

      function iniciarModo() {
        const llenar = document.getElementById("LLenar").checked;
        const remojar = document.getElementById("Remojar").checked;
        const lavar = document.getElementById("Lavar").checked;
        const enjuagar = document.getElementById("Enjuagar").checked;
        const exprimir = document.getElementById("Exprimir").checked;
        const centrifugado =
          document.getElementById("Centrifugado extra").checked;

        let opcionesActivadas = [];

        if (llenar) {
          opcionesActivadas.push("_1");
        }
        if (remojar) {
          opcionesActivadas.push("_2");
        }
        if (lavar) {
          opcionesActivadas.push("_3");
        }
        if (enjuagar) {
          opcionesActivadas.push("_4");
        }
        if (exprimir) {
          opcionesActivadas.push("_5");
        }
        if (centrifugado) {
          opcionesActivadas.push("_6");
        }

        const cadenaOpciones = opcionesActivadas.join("|");

        console.log(cadenaOpciones);
        consultaGET(cadenaOpciones);
      }

      function consultaGET(consulta) {
        var Http = new XMLHttpRequest();
        console.log(`Consultando  ${consulta}`);
        Http.open("GET", consulta, true);
        Http.send();

        Http.onreadystatechange = (e) => {
          console.log(Http.status);
          console.log(Http.responseText);
        };
      }
    </script>
  </body>
</html>)=====";