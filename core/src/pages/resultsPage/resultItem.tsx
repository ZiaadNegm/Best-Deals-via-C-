// ResultItem.js
import React, { useState } from 'react';
import Iphone from './placeholder/iPhone_15_Blue_PDP_Image_Position-1_alt__en-IN.webp';
import blackIphone from './placeholder/iPhone_16_Black_PDP_Image_Position_1b_Black_Color__TH-TH_1c13450b-b3a1-4534-8929-e142de60b150.webp';
import 'react-slideshow-image/dist/styles.css';
import { Slide } from 'react-slideshow-image';
import { Link } from 'react-router-dom';

type ResultItemProps = {
  id: number;
};

type PictureProps = {
  images: string[];
};

export function Picture({ images, id }: PictureProps & { id: number }) {
  const [currentSlide, setCurrentSlide] = useState(0);

  return (
    <div className="center-indicator w-full h-full">
      <div className="slide-indicator">
        {currentSlide + 1} / {images.length}
      </div>
      <div className="w-full h-full">
        <Slide
          onChange={(oldIndex, newIndex) => setCurrentSlide(newIndex)}
          transitionDuration={500}
        >
          {images.map((image, index) => (
            <div key={index} className="w-full h-full">
              <Link to={`/item/${id}`}>
                <img 
                  src={image} 
                  className="image w-full h-full" 
                  alt={`Slide ${index + 1}`} 
                />
              </Link>
            </div>
          ))}
        </Slide>
      </div>
    </div>
  );
}

export default function ResultItem({ id }: ResultItemProps) {
  const images = [Iphone, blackIphone];

  return (
    <div className="result-item">
      <div className="picture-frame">
        <Picture images={images} id={id} />
        <div className="result-information">
          <div className="info-block price-block">
            <p className="info-text">Price</p>
          </div>
          <div className="info-block score-block">
            <p className="info-text">Score</p>
          </div>
          <div className="info-block location-block">
            <p className="info-text">Location</p>
          </div>
          <div className="info-block offer-block">
            <button className="offer-button">Offer</button>
          </div>
        </div>
      </div>
    </div>
  );
}
