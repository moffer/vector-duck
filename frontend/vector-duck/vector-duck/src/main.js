import Vue from 'vue'
import './plugins/vuetify'
import App from './App.vue'
import 'roboto-fontface/css/roboto/roboto-fontface.css'
import 'material-design-icons-iconfont/dist/material-design-icons.css'

Vue.config.productionTip = false

import VueWebsocket from "vue-websocket";
// Vue.use(VueWebsocket, "ws://192.168.4.1:81");
// Vue.use(VueWebsocket, "ws://192.168.4.1:81/");

new Vue({
  render: h => h(App),
}).$mount('#app')
