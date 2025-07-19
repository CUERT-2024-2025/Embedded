#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>NodeMCU_Car</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body { 
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      color: #333;
    }

    .container {
      max-width: 500px;
      margin: 0 auto;
      padding: 20px;
    }

    .header { 
      background: rgba(255, 255, 255, 0.95);
      backdrop-filter: blur(10px);
      color: #333;
      padding: 20px;
      font-size: 24px;
      font-weight: 600;
      text-align: center;
      border-radius: 20px;
      margin-bottom: 30px;
      box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
      border: 1px solid rgba(255, 255, 255, 0.2);
    }

    .controls { 
      display: grid; 
      grid-template-columns: repeat(3, 1fr); 
      gap: 15px; 
      max-width: 300px;
      margin: 0 auto 30px;
    }

    .btn {
      background: rgba(255, 255, 255, 0.9);
      border: none;
      outline: none;
      cursor: pointer;
      border-radius: 20px;
      padding: 15px;
      transition: all 0.3s ease;
      box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
      border: 1px solid rgba(255, 255, 255, 0.2);

      /* ENABLE MULTI-TOUCH */
      touch-action: none;
      pointer-events: auto;
    }

    .btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 8px 25px rgba(0, 0, 0, 0.15);
      background: rgba(255, 255, 255, 1);
    }

    .btn svg { 
      width: 40px; 
      height: 40px;
      filter: drop-shadow(0 2px 4px rgba(0, 0, 0, 0.1));
    }

    .slider-container { 
      background: rgba(255, 255, 255, 0.95);
      backdrop-filter: blur(10px);
      border-radius: 25px;
      padding: 30px;
      box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
      border: 1px solid rgba(255, 255, 255, 0.2);
    }

    .slider-group {
      margin-bottom: 25px;
    }

    .slider-label { 
      font-size: 16px; 
      font-weight: 600; 
      margin-bottom: 12px; 
      color: #333;
      display: block;
    }

    .slider-value { 
      font-size: 14px; 
      color: #666; 
      margin-left: 10px;
      font-weight: 500;
    }

    .slider { 
      width: 100%;
      height: 8px;
      border-radius: 5px;
      background: #e0e0e0;
      outline: none;
      -webkit-appearance: none;
      margin: 10px 0;
    }

    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 24px;
      height: 24px;
      border-radius: 50%;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      cursor: pointer;
    }

    .toggle-row { 
      display: flex; 
      flex-direction: column;
      gap: 15px;
      margin-top: 20px;
    }

    .toggle-btn, .home-btn {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      border: none;
      border-radius: 15px;
      padding: 15px 25px;
      font-size: 16px;
      font-weight: 600;
      cursor: pointer;
      transition: all 0.3s ease;
    }

    .toggle-btn.active {
      background: linear-gradient(135deg, #ff6b6b 0%, #ee5a24 100%);
    }

    .home-btn:disabled {
      background: #ccc;
      cursor: not-allowed;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">NodeMCU Car Control</div>
    <div class="controls">
      <button class="btn" id="btnJ" onpointerdown="handleButtonPress('J', 'btnJ')" onpointerup="handleButtonRelease('btnJ')">
        <svg viewBox="0 0 60 60"><polygon points="30,50 50,30 30,30" fill="#667eea"/></svg>
      </button>
      <button class="btn" id="btnF" onpointerdown="handleButtonPress('F', 'btnF')" onpointerup="handleButtonRelease('btnF')">
        <svg viewBox="0 0 60 60"><polygon points="30,10 50,30 10,30" fill="#667eea"/></svg>
      </button>
      <button class="btn" id="btnH" onpointerdown="handleButtonPress('H', 'btnH')" onpointerup="handleButtonRelease('btnH')">
        <svg viewBox="0 0 60 60"><polygon points="30,50 10,30 30,30" fill="#667eea"/></svg>
      </button>
      <button class="btn" id="btnL" onpointerdown="handleButtonPress('L', 'btnL')" onpointerup="handleButtonRelease('btnL')">
        <svg viewBox="0 0 60 60"><polygon points="10,30 30,50 30,10" fill="#667eea"/></svg>
      </button>
      <button class="btn" id="btnS" onpointerdown="handleButtonPress('S', 'btnS')" onpointerup="handleButtonRelease('btnS')">
        <svg viewBox="0 0 60 60"><circle cx="30" cy="30" r="25" stroke="#667eea" stroke-width="5" fill="none"/></svg>
      </button>
      <button class="btn" id="btnR" onpointerdown="handleButtonPress('R', 'btnR')" onpointerup="handleButtonRelease('btnR')">
        <svg viewBox="0 0 60 60"><polygon points="50,30 30,50 30,10" fill="#667eea"/></svg>
      </button>
      <button class="btn" id="btnI" onpointerdown="handleButtonPress('I', 'btnI')" onpointerup="handleButtonRelease('btnI')">
        <svg viewBox="0 0 60 60"><polygon points="30,10 50,30 30,30" fill="#667eea"/></svg>
      </button>
      <button class="btn" id="btnB" onpointerdown="handleButtonPress('B', 'btnB')" onpointerup="handleButtonRelease('btnB')">
        <svg viewBox="0 0 60 60"><polygon points="30,50 50,30 10,30" fill="#667eea"/></svg>
      </button>
      <button class="btn" id="btnG" onpointerdown="handleButtonPress('G', 'btnG')" onpointerup="handleButtonRelease('btnG')">
        <svg viewBox="0 0 60 60"><polygon points="30,10 10,30 30,30" fill="#667eea"/></svg>
      </button>
    </div>

    <div class="slider-container">
      <div class="slider-group">
        <label class="slider-label">Car Speed</label>
        <input type="range" min="0" max="100" value="0" class="slider" id="speedSlider" oninput="updateSpeedValue(this.value); sendSpeed(this.value)">
        <span class="slider-value" id="speedValue">0%</span>
      </div>
      <div class="slider-group">
        <label class="slider-label">Steering Speed</label>
        <input type="range" min="1" max="100" value="75" class="slider" id="steerSlider" oninput="updateSteerValue(this.value); sendSteerSpeed(this.value)">
        <span class="slider-value" id="steerValue">75%</span>
      </div>
      <div class="toggle-row">
        <button id="autoHomeBtn" class="toggle-btn" onclick="toggleAutoHome()">Enable Auto Go Home</button>
        <button id="manualHomeBtn" class="home-btn" onclick="sendGoHome()">Go Home</button>
      </div>
    </div>
  </div>

  <script>
    const activeButtons = new Set();

    function sendCmd(cmd) {
      fetch('/cmd?dir=' + cmd);
    }

    function handleButtonPress(cmd, btnId) {
      if (activeButtons.has(btnId)) return;
      activeButtons.add(btnId);
      sendCmd(cmd);
    }

    function handleButtonRelease(btnId) {
      if (!activeButtons.has(btnId)) return;
      sendCmd('S');
      activeButtons.delete(btnId);
    }

    function sendSpeed(val) {
      fetch('/speed?car=' + val);
    }

    function sendSteerSpeed(val) {
      fetch('/speed?steer=' + val);
    }

    function updateSpeedValue(val) {
      document.getElementById('speedValue').textContent = val + '%';
    }

    function updateSteerValue(val) {
      document.getElementById('steerValue').textContent = val + '%';
    }

    function toggleAutoHome() {
      const btn = document.getElementById('autoHomeBtn');
      const manualBtn = document.getElementById('manualHomeBtn');
      const enabled = btn.classList.toggle('active');
      btn.textContent = enabled ? 'Disable Auto Go Home' : 'Enable Auto Go Home';
      manualBtn.disabled = enabled;
      if (enabled) {
        manualBtn.classList.remove('active');
      } else {
        manualBtn.classList.add('active');
      }
      fetch('/cmd?autoHome=' + (enabled ? '1' : '0'));
    }

    function sendGoHome() {
      fetch('/cmd?manualHome=1');
    }

    window.onload = function () {
      const btn = document.getElementById('autoHomeBtn');
      const manualBtn = document.getElementById('manualHomeBtn');
      if (!btn.classList.contains('active')) {
        manualBtn.disabled = false;
        manualBtn.classList.add('active');
      }
    }
  </script>
</body>
</html>
)rawliteral";

#endif
