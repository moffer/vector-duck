<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 mb-5>
        <h2 class="headline font-weight-bold mb-3">Steuerung</h2>

        <v-layout justify-center style="flex-flow:column" class="layout">
          <v-btn @click="forward()">
            <v-icon>arrow_upward</v-icon>
          </v-btn>
          <div>
            <v-btn @click="left()">
              <v-icon>arrow_back</v-icon>
            </v-btn>
            <v-btn @click="gyro()">
              <v-icon>replay</v-icon>
            </v-btn>
            <v-btn @click="right()">
              <v-icon>arrow_forward</v-icon>
            </v-btn>
          </div>
          <v-btn @click="backwards()">
            <v-icon>arrow_downward</v-icon>
          </v-btn>
          <v-btn @click="stop()">
            <v-icon>stop</v-icon>
          </v-btn>
        </v-layout>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
export default {
  data: () => ({
    websocket: null
  }),
  mounted() {
    this.websocket = new WebSocket("ws://192.168.4.1:81/");
  },
  methods: {
    add() {
      this.websocket.send("asdf");
    },
    forward() {
      console.log('vor')
      this.websocket.send("F");
    },
    backwards() {
      console.log('rück')
      this.websocket.send("B");
    },
    left() {
      console.log('left')
      this.websocket.send("L");
    },
    right() {
      console.log('rechts')
      this.websocket.send("R");
    },
    gyro() {
      this.websocket.send("X");
    },
    stop() {
      console.log('stop')
      this.websocket.send("S");
    }
  }
};
</script>

<style>
.layout {
  flex-flow: column;
  align-items: center;
}
button {
  width: fit-content;
}
</style>
