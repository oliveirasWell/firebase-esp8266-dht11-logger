import getMuiTheme from 'material-ui/styles/getMuiTheme';
import {grey900, grey800} from "material-ui/styles/colors";

const theme = getMuiTheme({
    palette: {
        primary1Color: grey900,
        primary2Color: grey800,
    },
});

export default theme;