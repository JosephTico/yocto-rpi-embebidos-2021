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
  <div class="graphic">
    <img src="./assets/casita.svg" alt="My casita" />
    <div class="door" id="d1"></div>
    <div class="door2" id="d2"></div>
    <div class="door2" id="d3"></div>
    <div class="door horizontal" id="d4"></div>
    <label class="switch" id="s1">
      <input type="checkbox" v-model="leds[0].on" />
      <span class="slider round"></span>
    </label>
    <label class="switch" id="s2">
      <input type="checkbox" v-model="leds[1].on" />
      <span class="slider round"></span>
    </label>
    <label class="switch" id="s3">
      <input type="checkbox" v-model="leds[2].on" />
      <span class="slider round"></span>
    </label>
    <label class="switch" id="s4">
      <input type="checkbox" v-model="leds[3].on" />
      <span class="slider round"></span>
    </label>
    <label class="switch" id="s5">
      <input type="checkbox" v-model="leds[4].on" />
      <span class="slider round"></span>
    </label>
  </div>
  

  <br />
  <!--ul id="example-1">
  Doors: {{ doors }}<br />
  
    <li v-for="led in leds" :key="led.id">
      {{ led.id }}: {{ led.on ? "on" : "off" }}
      
      <label class="switch">
        <input type="checkbox" v-model="led.on" />
        <span class="slider round"></span>
      </label>
    </li>
  </ul>
  < Rounded switch -->
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

.graphic{
  position: relative;
  max-height: 90vh;
}

.graphic img {
  max-width: 100%;
  max-height: 100%;
}

#s1{
  position: absolute;
  top: 25%;
  left: 22%;
}

#s2{
  position: absolute;
  top: 70%;
  left: 22%;
}

#s3{
  position: absolute;
  top: 76%;
  left: 57%;
}

#s4{
  position: absolute;
  top: 40%;
  left: 59%;
}

#s5{
  position: absolute;
  top: 10%;
  left: 57%;
}

.door:hover {
    background: green;
    transform: rotate(-90deg);
}
.door2:hover {
    background: green;
    transform: rotate(90deg);
}
.door.horizontal:hover {
    background: green;
    transform: rotate(90deg);
}
.door.horizontal {
   background: #000;
    width: 5vw;
    height: 0.5vw;
    position: absolute;
    left: 0;
    top: 0;
    transition: ease 1s;
    transform-origin: 100% 0%;
    background: red;
}
.door {
    background: #000;
    width: 0.5vw;
    height: 5vw;
    position: absolute;
    left: 0;
    top: 0;
    transition: ease 1s;
    transform-origin: 0% 100%;
    background: red;
}
.door2 {
    background: #000;
    width: 0.5vw;
    height: 5vw;
    position: absolute;
    left: 0;
    top: 0;
    transition: ease 1s;
    transform-origin: 100% 0%;
    background: red;
}

#d1 {
  left: 38.3vw;
  top: 11.5vw
}
#d2 {
  left: 38.3vw;
  top: 21.2vw;
 
}
#d3 {
  left: 67.0vw;
  top: 2.5vw
}
#d4 {
  left: 42vw;
  top: 36.1vw
}




</style>
