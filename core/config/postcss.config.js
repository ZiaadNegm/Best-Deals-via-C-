// config/postcss.config.js

import path from 'path';

/** @type {import('postcss').Config} */
export default {
  plugins: {
    tailwindcss: { config: path.resolve(__dirname, 'tailwind.config.js') }, // Adjusted path
    autoprefixer: {},
  },
}
