import React from "react";
import "../css/App.css";
import MuiThemeProvider from "material-ui/styles/MuiThemeProvider";
import {AppBar, Drawer, MenuItem} from "material-ui";
import theme from "../utils/theme";

const App = React.createClass({
    getInitialState() {
        return {
            open: false,
        };
    },
    handleToggle() {
        this.setState({open: !this.state.open});
    },
    render() {
        return (
            <MuiThemeProvider muiTheme={theme}>
                <div>
                    <AppBar
                        title="Leituras"
                        iconClassNameRight="muidocs-icon-navigation-expand-more"
                        onLeftIconButtonTouchTap={this.handleToggle}
                    />
                    {this.props.children}
                    <Drawer open={this.state.open}>
                        <AppBar
                            title="Leituras"
                            iconClassNameRight="muidocs-icon-navigation-expand-more"
                            onLeftIconButtonTouchTap={this.handleToggle}
                        />
                        <MenuItem >Menu</MenuItem>
                        <MenuItem>Menu Item 2</MenuItem>
                    </Drawer>
                </div>
            </MuiThemeProvider>
        );
    }
});

export default App;


