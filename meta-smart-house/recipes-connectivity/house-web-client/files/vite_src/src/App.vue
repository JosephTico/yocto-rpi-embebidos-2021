<script setup>
import { ref, onBeforeUnmount, watch } from "vue";

let doors = ref({});
let leds = ref([
  { id: 1, on: false },
  { id: 2, on: false },
  { id: 3, on: false },
  { id: 4, on: false },
  { id: 5, on: false },
]);
let polling = null;

const reload = async () => {
  const response = await fetch("/doors");
  const data = await response.json();
  doors.value = data;
};

const sendLedData = async () => {
  // post to server
  const url = "/leds";
  const response = await fetch(url, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(leds.value),
  });
};

const modifyAllLeds = async (status) => {
  leds.value.forEach((led) => {
    led.on = status;
  });
};

function set_interval() {
  polling = setInterval(() => {
    reload();
  }, 150);
}

set_interval();
sendLedData();

watch(leds.value, sendLedData);

onBeforeUnmount(() => {
  clearInterval(polling);
});
</script>

<template>
  <img src="./assets/casita.svg" alt="My casita" />
  <img alt="Vue logo" src="./assets/logo.png" />
  <br />
  Doors: {{ doors }}<br />
  <ul id="example-1">
    <li v-for="led in leds" :key="led.id">
      {{ led.id }}: {{ led.on ? "on" : "off" }}
      <!-- Rounded switch -->
      <label class="switch">
        <input type="checkbox" v-model="led.on" />
        <span class="slider round"></span>
      </label>
    </li>
  </ul>
  <button v-on:click="modifyAllLeds(false)">Turn off all</button>
  <button v-on:click="modifyAllLeds(true)">Turn on all</button>
</template>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
/* The switch - the box around the slider */
.switch {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

/* Hide default HTML checkbox */
.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

/* The slider */
.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: 0.4s;
  transition: 0.4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: 0.4s;
  transition: 0.4s;
}

input:checked + .slider {
  background-color: #2196f3;
}

input:focus + .slider {
  box-shadow: 0 0 1px #2196f3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
</style>
