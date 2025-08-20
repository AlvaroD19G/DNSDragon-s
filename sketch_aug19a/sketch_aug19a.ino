#include <WiFi.h>
#include <WebServer.h>

// ---------- Pines ----------
#define POT 34
#define LED_VERDE 25
#define LED_AZUL 26
#define LED_ROJO 27
#define BUZZER 32

// ---------- Variables de medida ----------
int lastValue = 0;
unsigned long lastTime = 0;
float velocidad = 0;  // unidades arbitrarias (u/s)

// ---------- WiFi AP + Web ----------
const char* AP_SSID = "ESP32-Bus";
const char* AP_PASS = "12345678";  // >=8 chars
WebServer server(80);

// ---------- HTML/CSS/JS ----------
const char INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html lang="es">
<head>
<meta charset="utf-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1"/>
<title>School Bus Speed</title>
<style>
  :root{
    --yellow:#ffc51c; --yellow-d:#e7ae00; --black:#0a0a0a; --metal:#7c8794;
    --bg:#0b0f1a; --green:#27ae60; --blue:#2980b9; --red:#e74c3c;
  }
  *{box-sizing:border-box}
  body{
    margin:0; min-height:100vh; display:grid; place-items:center; background:#0b0f1a;
    color:#fff; font-family:system-ui,Segoe UI,Arial; overflow:hidden;
    background-image:
      radial-gradient(1200px 600px at 50% -10%, #13213a 0%, #0b0f1a 60%, #06080f 100%),
      repeating-linear-gradient( 75deg, transparent 0 36px, rgba(255,255,255,.05) 36px 40px),
      repeating-linear-gradient(-75deg, transparent 0 30px, rgba(255,255,255,.035) 30px 34px);
    animation: fly 1.2s linear infinite;
  }
  @keyframes fly{
    from{background-position:0 0, 0 0, 0 0}
    to  {background-position:0 0, 420px -240px, -360px 260px}
  }

  /* ======== BUS (estructura general) ======== */
  .bus{
    width:min(94vw,880px); aspect-ratio:4/3; position:relative; border-radius:26px;
    background:linear-gradient(#ffd84f,var(--yellow) 60%,#ffcf1a);
    box-shadow:0 22px 60px rgba(0,0,0,.45), inset 0 0 0 6px #c69200;
  }
  .cap, .skirt{
    position:absolute; left:0; right:0; height:28px; background:linear-gradient(#ffd44a,#f0bd1a);
  }
  .cap{top:0; box-shadow:inset 0 -4px 8px rgba(0,0,0,.25)}
  .skirt{bottom:0; box-shadow:inset 0 4px 8px rgba(0,0,0,.25)}

  /* Espejos laterales */
  .mirror{position:absolute; top:92px; width:24px; height:120px; background:#1e232b; border-radius:6px;
          box-shadow:inset 0 0 0 3px #2b313c}
  .mirror.left{left:-20px} .mirror.right{right:-20px}
  .mirror::after{content:""; position:absolute; top:10px; left:50%; transform:translateX(-50%);
                 width:14px; height:100px; background:#0b0e13; border-radius:4px}

  /* Bumper y ruedas */
  .bumper{
    position:absolute; left:6%; right:6%; bottom:-14px; height:34px; border-radius:10px;
    background:linear-gradient(#aab2ba,#76808b); box-shadow:0 10px 18px rgba(0,0,0,.35);
  }
  .wheel{position:absolute; bottom:-34px; width:58px; height:58px; border-radius:50%;
         background:radial-gradient(circle at 35% 35%, #444 0 18%, #222 19% 70%, #000 71% 100%);
         box-shadow:0 10px 20px rgba(0,0,0,.45)}
  .wheel.left{left:70px} .wheel.right{right:70px}

  /* Banner SCHOOL BUS */
  .banner{
    position:absolute; left:12%; right:12%; top:26px; height:54px; border-radius:14px;
    background:#20242c; display:grid; place-items:center; font-weight:900; letter-spacing:2px;
    box-shadow:inset 0 0 0 3px #2e3440, 0 8px 20px rgba(0,0,0,.2);
  }

  /* Parabrisas y tablero */
  .bezel{
    position:absolute; left:6%; right:6%; top:92px; bottom:116px; border-radius:22px;
    background:linear-gradient(#172231,#0e1623); box-shadow:inset 0 0 0 4px #1f2b3b;
    padding:18px; display:grid; place-items:center;
  }
  .windshield{
    width:100%; height:100%; border-radius:18px;
    background:radial-gradient(140% 120% at 50% -20%, rgba(0,180,255,.14), transparent 55%),
               linear-gradient(#0c1a27,#060b12);
    border:4px solid #243241; display:grid; place-items:center; padding:14px;
    position:relative; overflow:hidden;
    box-shadow:inset 0 0 120px rgba(0,0,0,.6);
  }
  /* Reflejos diagonales en el vidrio */
  .windshield::before, .windshield::after{
    content:""; position:absolute; inset:-20% -10%;
    background:linear-gradient(120deg, rgba(255,255,255,.10) 0%, rgba(255,255,255,0) 30%);
    transform:skewX(-12deg) translateY(-12%);
    pointer-events:none;
  }
  .windshield::after{ transform:skewX(-12deg) translateY(22%); opacity:.6 }

  .dash{
    width:92%; height:78%; border-radius:16px;
    background:linear-gradient(180deg,#0b253d,#071625);
    border:3px solid #29435c; display:grid; place-items:center; text-align:center;
    box-shadow:inset 0 0 60px rgba(0,0,0,.55);
  }

  /* Valor grande y unidades */
  .speed{
    font-size:clamp(44px, 9.6vw, 110px); font-weight:900; letter-spacing:1px; line-height:1;
    color:#fff; text-shadow:0 0 18px rgba(0,209,255,.55);
  }
  .units{ display:block; margin-top:6px; font-size:clamp(12px,2.4vw,16px); opacity:.9; font-weight:800; }

  /* Panel inferior: barra + chip de nivel */
  .panel{
    position:absolute; left:8%; right:8%; bottom:46px; display:flex; gap:16px; align-items:center;
  }
  .rail{flex:1; height:16px; background:#0f1520; border-radius:999px; border:2px solid #243247; overflow:hidden}
  .fill{height:100%; width:0%}
  .badge{
  position:absolute;
  top:84px;   /* debajo del banner */
  left:50%;
  transform:translateX(-50%);
  min-width:130px;
  text-align:center;
  padding:10px 16px;
  border-radius:999px;
  font-weight:900;
  background:#0f1520;
  border:2px solid #243247;
  color:#e8eefc;
  letter-spacing:.5px;
  box-shadow:0 6px 14px rgba(0,0,0,.25);
  z-index:20;
  font-size:1rem;
}
  /* Parrilla y luces */
  .grille{
    position:absolute; left:24%; right:24%; bottom:30px; height:40px; border-radius:10px;
    background:linear-gradient(#23262d,#15191f); box-shadow:inset 0 0 0 3px #2a2e36;
  }
  .slots{position:absolute; inset:8px; display:grid; grid-template-columns:repeat(8,1fr); gap:10px}
  .slots span{ background:#0e1116; border-radius:6px; box-shadow:inset 0 2px 0 rgba(255,255,255,.06) }

  .lamp{ position:absolute; bottom:54px; width:64px; height:42px; border-radius:50%;
    background: radial-gradient(circle at 40% 40%, #fff6ce 0 55%, #c6b783 70% 100%);
    box-shadow:0 0 28px rgba(255,230,170,.55) }
  .lamp.left{ left:34px } .lamp.right{ right:34px }

  .turn{ position:absolute; bottom:66px; width:20px; height:20px; border-radius:4px; background:#ffae2b;
         box-shadow:0 0 16px rgba(255,174,43,.7) }
  .turn.left{ left:110px } .turn.right{ right:110px }

  /* Colores por nivel */
  .lvl-slow  .fill{ background:linear-gradient(90deg,var(--green),#42e28f) }
  .lvl-mid   .fill{ background:linear-gradient(90deg,var(--blue), #6fb9ff) }
  .lvl-fast  .fill{ background:linear-gradient(90deg,var(--red),  #ff7a6a) }
  .lvl-slow  .badge{ color:#caffdd } .lvl-mid .badge{ color:#d9e8ff } .lvl-fast .badge{ color:#ffe3de }
</style>
</head>
<body>

  <div id="bus" class="bus lvl-slow">
    <div class="cap"></div>
    <div class="skirt"></div>

    <!-- Espejos laterales -->
    <div class="mirror left"></div>
    <div class="mirror right"></div>

    <div class="banner">SCHOOL BUS</div>
    <div id="nivel" class="badge">LENTO</div>
    <div class="bezel">
      <div class="windshield">
        <div class="dash">
          <div class="speed" id="vel">0.0<span class="units"> u/s</span></div>
        </div>
      </div>
    </div>

    <div class="panel">
      <div class="rail"><div id="bar" class="fill"></div></div>
      <div id="nivel" class="badge">LENTO</div>
    </div>

    <div class="grille">
      <div class="slots">
        <span></span><span></span><span></span><span></span>
        <span></span><span></span><span></span><span></span>
      </div>
    </div>

    <div class="lamp left"></div>
    <div class="lamp right"></div>
    <div class="turn left"></div>
    <div class="turn right"></div>

    <div class="wheel left"></div>
    <div class="wheel right"></div>
    <div class="bumper"></div>
  </div>

<script>
async function tick(){
  try{
    const r = await fetch('/data',{cache:'no-store'});
    const j = await r.json();
    const v = j.velocidad||0;

    const vel = document.getElementById('vel');
    const bar = document.getElementById('bar');
    const nivel = document.getElementById('nivel');
    const bus = document.getElementById('bus');

    vel.firstChild.nodeValue = v.toFixed(1);
    const pct = Math.max(0, Math.min(100, (v/500)*100));
    bar.style.width = pct + '%';

    bus.classList.remove('lvl-slow','lvl-mid','lvl-fast');
    if(v < 100){ bus.classList.add('lvl-slow'); nivel.textContent='LENTO'; }
    else if(v < 300){ bus.classList.add('lvl-mid'); nivel.textContent='MEDIO'; }
    else { bus.classList.add('lvl-fast'); nivel.textContent='RÁPIDO'; }
  }catch(e){}
}
setInterval(tick,200); tick();
</script>
</body>
</html>
)HTML";

// ---------- Handlers ----------
void handleData() {
  String json = "{\"velocidad\":" + String(velocidad, 2) + "}";
  server.send(200, "application/json", json);
}
void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void setup() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  analogReadResolution(12);

  Serial.begin(115200);
  delay(150);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(300);
  Serial.println("AP activo");
  Serial.print("Conectate a: ");
  Serial.println(AP_SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());  // 192.168.4.1

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();

  lastTime = millis();
}

void loop() {
  // Medición de “velocidad”
  int value = analogRead(POT);
  unsigned long now = millis();
  int diff = abs(value - lastValue);
  unsigned long dt = now - lastTime;

  if (dt >= 100) {
    velocidad = (diff * 1000.0) / dt;
    lastValue = value;
    lastTime = now;
  }

  // LEDs por nivel
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_ROJO, LOW);
  if (velocidad < 100) digitalWrite(LED_VERDE, HIGH);
  else if (velocidad < 300) digitalWrite(LED_AZUL, HIGH);
  else digitalWrite(LED_ROJO, HIGH);

  // Buzzer
  int delayTime = map((int)velocidad, 0, 500, 500, 60);
  delayTime = constrain(delayTime, 40, 600);
  tone(BUZZER, 1000);
  delay(delayTime);
  noTone(BUZZER);
  delay(delayTime);

  server.handleClient();
}
