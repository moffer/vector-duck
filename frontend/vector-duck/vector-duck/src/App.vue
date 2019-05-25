<template>
  <v-app>
    <v-toolbar app class="toolbar">
      <img src="./assets/Vector_Informatik_Logo.svg.png" style="height: 100%">
      <v-toolbar-title class="headline text-uppercase" style="margin-left: -15px;">
        <!-- <span>Vector</span> -->
        <span class="font-weight-light">Duck</span>
      </v-toolbar-title>
    </v-toolbar>

    <v-content>
      <HelloWorld/>
    </v-content>
  </v-app>
</template>

<script>
import HelloWorld from "./components/HelloWorld";

export default {
  name: "App",
  components: {
    HelloWorld
  },
  data() {
    return {
      //
    };
  },
  methods: {
    add() {}
  }
};
</script>
<style>
.toolbar > div {
  justify-content: center;
}
</style>

