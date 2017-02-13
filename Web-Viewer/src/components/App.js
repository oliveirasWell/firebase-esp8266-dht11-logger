import React from 'react';
import '../css/App.css';
import MuiThemeProvider from 'material-ui/styles/MuiThemeProvider';
import Logger from './Logger.js';
import theme from '../utils/theme';

const App = React.createClass({
    render() {
        return (
            <MuiThemeProvider muiTheme={theme}>
                <Logger/>
            </MuiThemeProvider>
        );
    }
});

export default App;


