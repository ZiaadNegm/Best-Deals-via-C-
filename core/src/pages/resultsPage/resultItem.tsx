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

export function Picture({ images }: PictureProps) {
  const [currentSlide, setCurrentSlide] = useState(0);

  return (
    <div className="center-indicator">
      <div className="slide-indicator">
        {currentSlide + 1} / {images.length}
      </div>
      <Slide
        onChange={(oldIndex, newIndex) => setCurrentSlide(newIndex)}
        transitionDuration={500}
      >
        {images.map((image, index) => (
          <div key={index}>
            <img src={image} className="image" alt={`Slide ${index + 1}`} />
          </div>
        ))}
      </Slide>
    </div>
  );
}

export default function ResultItem({ id }: ResultItemProps) {
  const [currentSlide, setCurrentSlide] = useState(0);
  const images = [Iphone, blackIphone];

  return (
    <div className="result-item">
      <div className="picture-frame">
      <Link to={`/item/${id}`}>
          <Picture images={images} />
        </Link>
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
