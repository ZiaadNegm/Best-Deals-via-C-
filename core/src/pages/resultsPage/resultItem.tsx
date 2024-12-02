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
        <div className="text-center mb-4">
          <div className="inline-flex items-center justify-center bg-black shadow-md bg-opacity-50 text-white px-2 py-1 rounded">
            {currentSlide + 1} / {images.length}
          </div>
        </div>
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
        <div className="grid grid-cols-2 grid-rows-2 h-full w-full">
          <div className="row-start-1 col-start-1 border border-gray-300 p-4 flex items-center justify-center">
              <p className="text-5xl font-semibold">Price</p>
          </div>
          <div className="row-start-1 col-start-2 border border-gray-300 p-4 flex items-center justify-center">
              <p className="text-5xl font-semibold">Score</p>
          </div>
          <div className="row-start-2 col-start-1 border border-gray-300 p-4 flex items-center justify-center">
              <p className="text-5xl font-semibold mb-8">Location</p>
          </div>
          <div className="row-start-2 col-start-2 border border-gray-300 p-4 flex items-center justify-center">
              <p className="text-5xl font-semibold mb-8">Offer</p>
          </div>
        </div>

      </div>
    </div>
  );
}
