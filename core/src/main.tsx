import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { createBrowserRouter, Route, RouterProvider } from 'react-router-dom'
import {Action as SubmitAction} from './components/sharedFilters/submit.tsx'
import ErrorPage from './pages/errorPage/errorPage.tsx'
import './index.css'
import App from './App.tsx'
import React from 'react'


const router = createBrowserRouter([
  {
    path: "/",
    element:<App/>,
    action: SubmitAction,
    errorElement: <ErrorPage/>,
  },
])


createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <RouterProvider router={router}/>
  </StrictMode>,
)

