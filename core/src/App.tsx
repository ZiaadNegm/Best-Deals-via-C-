// src/App.js
import React from 'react';
import './App.css';
import ParticleBackground from './style/css-particle-animation-no-javascript/ParticleBackground';
import MainPage from './pages/mainPage/core_mainPage'

function App() {
  return (
    <div>
      <ParticleBackground />
      <MainPage />
    </div>
  );
}

export default App;
