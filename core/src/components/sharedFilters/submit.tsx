import React from 'react';
import { Form, redirect } from 'react-router-dom';


export function Action(){
    return redirect("/results");
}

export default function Submit(){
    return(<Form method="post">
        <button className='rounded_full_bars text-5xl'>Submit</button>
    </Form>
    )
}
