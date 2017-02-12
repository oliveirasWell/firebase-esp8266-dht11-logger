import * as firebase from "firebase";
import ReactFireMixin from "reactfire";
import React from 'react';
import {AppBar, Tabs, Tab} from 'material-ui'
import config from './auth.js'
import ListLeitura from './ListLeitura.js';

const Logger = React.createClass({
    mixins: [ReactFireMixin],
    getInitialState() {
        return {
            data: []
        };
    },
    componentWillMount() {
        firebase.initializeApp(config);
        this.bindAsArray(firebase.database().ref('leitura').limitToLast(10) /*.orderByChild('data')*/, 'data');
    },
    render(){
        return (
            <div>
                <AppBar
                    title="Leituras"
                    iconClassNameRight="muidocs-icon-navigation-expand-more"
                >
                    <Tabs>
                        <Tab label="Leituras">
                        </Tab>
                    </Tabs>

                </AppBar>
                <ListLeitura data={this.state.data}/>
            </div>
        );
    }
});

export default Logger;