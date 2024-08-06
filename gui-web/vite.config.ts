import { fileURLToPath, URL } from 'node:url'

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'
import { viteSingleFile } from "vite-plugin-singlefile"
// https://vitejs.dev/config/
export default defineConfig({
  build: {
    assetsInlineLimit: 99999999,
  },
  plugins: [
    vue(),
    viteSingleFile(),
    vueDevTools(),
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  },
  server: {
    port: 8531,
    strictPort: true
  }
})
