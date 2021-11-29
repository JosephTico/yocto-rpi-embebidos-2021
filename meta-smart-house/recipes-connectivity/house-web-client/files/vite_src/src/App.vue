<script setup>
import { ref, onBeforeUnmount, watch } from "vue";

let gpio = ref(false);
let files = ref([]);
let polling = null;

const reload = async () => {
  let response = await fetch("/gpio");
  let data = await response.json();
  gpio.value = data == "1";

  response = await fetch("/cam");
  data = await response.json();
  files.value = data.reverse();
};

const insertPhoto = async () => {
  let img = new Image();
  let div = document.getElementById("camera-photo");

  img.onload = function () {
    div.appendChild(img);
  };

  img.src = "/camera?t=" + new Date().getTime();
};

function set_interval() {
  polling = setInterval(() => {
    reload();
  }, 150);
}

set_interval();

onBeforeUnmount(() => {
  clearInterval(polling);
});
</script>

<template>
  <nav class="navbar navbar-dark bg-dark mb-4">
    <div class="container-fluid">
      <span class="navbar-brand mb-0 h1">Security System ACME v0.1</span>
    </div>
  </nav>

  <div class="container-lg">
    <div class="alert alert-danger" role="alert" v-if="gpio">
      Se ha detectado movimiento.
    </div>

    <div class="row row-cols-1 row-cols-md-2 g-4">
      <div class="col" v-for="file in files">
        <div class="card">
          <img :src="file.url" class="card-img-top" />
          <div class="card-body">
            <h5 class="card-title">{{ file.time }}</h5>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 0px;
}
</style>
