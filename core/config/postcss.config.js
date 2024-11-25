// config/postcss.config.js

import tailwindcss from 'tailwindcss';
import autoprefixer from 'autoprefixer';
import path from 'path';
import { fileURLToPath } from 'url';

// Derive __dirname in ES Modules
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export default {
  plugins: [
    tailwindcss({
      config: path.resolve(__dirname, 'tailwind.config.js'),
    }),
    autoprefixer(),
  ],
};
