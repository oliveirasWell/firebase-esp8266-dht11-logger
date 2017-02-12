import React from 'react';
import './App.css';
import MuiThemeProvider from 'material-ui/styles/MuiThemeProvider';
import Logger from './Logger.js';

const App = React.createClass({
    render() {
        return (
            <MuiThemeProvider>
                <Logger/>
            </MuiThemeProvider>
        );
    }
});

export default App;


