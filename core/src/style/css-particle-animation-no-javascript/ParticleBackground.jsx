// src/ParticleBackground.js
import React from 'react';
import './src/style.css';

function ParticleBackground() {
  const particles = Array.from({ length: 300 });

  return (
    <div className="container">
      {particles.map((_, i) => (
        <div className="circle-container" key={i}>
          <div className="circle"></div>
        </div>
      ))}
    </div>
  );0
}

export default ParticleBackground;
