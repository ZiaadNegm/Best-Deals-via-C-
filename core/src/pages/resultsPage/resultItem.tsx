// ResultItem.js
import React, { useState } from 'react';
import Iphone from './placeholder/iPhone_15_Blue_PDP_Image_Position-1_alt__en-IN.webp';
import blackIphone from './placeholder/iPhone_16_Black_PDP_Image_Position_1b_Black_Color__TH-TH_1c13450b-b3a1-4534-8929-e142de60b150.webp';
import 'react-slideshow-image/dist/styles.css';
import { Slide } from 'react-slideshow-image';

export default function ResultItem() {
  const [currentSlide, setCurrentSlide] = useState(0);
  const images = [Iphone, blackIphone];

  return (
    <div className="result-item">
      <div className="picture-frame relative">
        {/* Center the slide indicator */}
        <div className="text-center mb-4">
          <div className="inline-flex items-center justify-center bg-black shadow-md bg-opacity-50 text-white px-2 py-1 rounded">
            {currentSlide + 1} / {images.length}
          </div>
        </div>
        {/* Slideshow */}
        <Slide
          onChange={(oldIndex, newIndex) => setCurrentSlide(newIndex)}
          transitionDuration={500}
        >
          {images.map((image, index) => (
            <div key={index}>
              <img src={image} className="image h-full w-full" alt={`Slide ${index + 1}`} />
            </div>
          ))}
        </Slide>
        {/* Product Details */}
        <div className="p-4">
          <h3 className="text-lg font-semibold">Product Name</h3>
          <p className="text-gray-600">Price: $XX.XX</p>
        </div>
      </div>
    </div>
  );
}
