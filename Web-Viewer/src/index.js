import React from 'react';
import ReactDOM from 'react-dom';
import App from './components/App';
import ListLeitura from "./components/ListLeitura";
import {Router, Route, browserHistory} from 'react-router';
import injectTapEventPlugin from 'react-tap-event-plugin';
import './css/index.css';

injectTapEventPlugin();

ReactDOM.render(
    <Router history={browserHistory}>
            <Route path="/" component={App}>
                <Route path="/leituras" component={ListLeitura}/>
            </Route>
        </Router>,
    document.getElementById('root')
);
