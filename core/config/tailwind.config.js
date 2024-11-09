// config/tailwind.config.js

const path = require('path');

/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    path.resolve(__dirname, '../index.html'), // Adjusted path
    path.resolve(__dirname, '../src/**/*.{js,ts,jsx,tsx}'), // Adjusted path
  ],
  theme: {
    extend: {},
  },
  plugins: [],
}
