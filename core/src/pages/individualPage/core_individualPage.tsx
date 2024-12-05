import React from 'react';
import Header from '../resultsPage/header'
import Placeholder from '../resultsPage/placeholder/iPhone_16_Black_PDP_Image_Position_1b_Black_Color__TH-TH_1c13450b-b3a1-4534-8929-e142de60b150.webp'
import { Slide } from 'react-slideshow-image';
import { useState } from 'react';

export default function IndividualPage(){
    const [currentslide, setCurrentSlide] = useState(0);
    return(
        <div>
            <Header/>
            <div className='grid-2x1_2x1Grid '>
                <div className='col-start-1 row-start-1 border border-gray-500'>
                </div>
                <div className='col-start-2 row-start-1 row-span-2 border border-gray-500'>
                    <p>Haii</p>
                </div>
                <div className='col-start-1 row-start-2 border border-gray-500'>
                    <p>Haiii</p>

                </div>
            </div> 
        </div>
    )
}
