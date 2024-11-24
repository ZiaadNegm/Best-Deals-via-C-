import React from "react";
import { useRouteError } from "react-router-dom";


export default function ErrorPage(){
    const error = useRouteError();
    let errorMessage: string = "An unexpected error has occured.";

    if(error instanceof Error){
        errorMessage = error.message;
    } else if(typeof error === "object" && error != null){
        if("statusText" in error && typeof(error as any).message === "string"){
            errorMessage = (error as any).statusText;
        } else if("message" in error && typeof (error as any).message === "string"){
            errorMessage = (error as any).message
        }
    }

    return(
        <div className="center">
            <h1 className="text_color_big">Something went wrong.</h1>
            <strong className="text_color_smaller">{errorMessage}</strong>
        </div>
    )
};
