// ResultItem.js
import React from 'react';
import Iphone from './placeholder/iPhone_15_Blue_PDP_Image_Position-1_alt__en-IN.webp';
import black_iphone from './placeholder/iPhone_16_Black_PDP_Image_Position_1b_Black_Color__TH-TH_1c13450b-b3a1-4534-8929-e142de60b150.webp';
import 'react-slideshow-image/dist/styles.css';
import { Slide } from 'react-slideshow-image';

export default function ResultItem() {
  // Store the images in an array
  const images = [Iphone, black_iphone];

  return (
    <div className='result-item'>
      <div className="picture-frame">
        <Slide
        transitionDuration={500}>
        {images.map((image, index) => (
            <div key={index}>
              <img
                src={image}
                className='image h-full w-full'
                />
            </div>
          ))}
        </Slide>
        <div className="p-4">
          <h3 className="text-lg font-semibold">Product Name</h3>
          <p className="text-gray-600">Price: $XX.XX</p>
          {/* Add more information as needed */}
        </div>
      </div>
    </div>
  );
}
