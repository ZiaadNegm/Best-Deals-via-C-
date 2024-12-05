// IndividualPage.jsx
import React, { useState } from 'react';
import Header from '../resultsPage/header';
import Placeholder from '../resultsPage/placeholder/iPhone_15_Blue_PDP_Image_Position-1_alt__en-IN.webp';
import { useParams } from 'react-router-dom';
import { Slide } from 'react-slideshow-image';

type IdProps = {
  id: number;
  images: string[];
};

function Pill({ placeholder, value }: { placeholder: string; value?: string }) {
  const displayText = value && value.trim() !== "" ? value : placeholder;
  return (
    <p className="rounded-full bg-white text-gray-300 text-4xl px-6 py-6 inline-block">
      {displayText}
    </p>
  );
}

function Picture({ id, images }: IdProps) {
  const [currentSlide, setCurrentSlide] = useState(0);

  return (
    <div className="center-indicator w-full h-full relative">
      <div className="slide-indicator">
        {currentSlide + 1} / {images.length}
      </div>
      <Slide
        onChange={(oldIndex: number, newIndex: number) => setCurrentSlide(newIndex)}
        transitionDuration={500}
        cssClass="w-full h-full"
      >
        {images.map((image, index) => (
          <div key={index} className="flex items-center justify-center w-full h-full">
            <img
              src={image}
              className="object-cover"
              alt={`Slide ${index}`}
            />
          </div>
        ))}
      </Slide>
    </div>
  );
}

export default function IndividualPage() {
  const images = [Placeholder, Placeholder];
  const { id } = useParams<{ id: string }>();
  const numericId = id ? parseInt(id, 10) : 0;

  const originalLinkValue = "";
  const scoreValue = "9.5";
  const priceValue = "";
  const estWorthValue = "€200"; 
  const reviewsValue = "";

  return (
    <div>
      <Header />
      <div className="grid-2x1_2x1Grid w-full h-screen border border-gray-500">
        <div className="col-start-1 row-start-1 border border-gray-500 overflow-hidden relative w-full h-full">
          <div className="picture-frame w-full h-full">
            <Picture id={numericId} images={images} />
          </div>
        </div>

        <div className="flex flex-col space-y-4 p-4">
          <Pill placeholder="Original Link:" value={originalLinkValue} />
          <Pill placeholder="Score:" value={scoreValue} />
          <Pill placeholder="Price:" value={priceValue} />
          <Pill placeholder="Est worth:" value={estWorthValue} />
          <Pill placeholder="Reviews:" value={reviewsValue} />
        </div>
      </div>
    </div>
  );
}
