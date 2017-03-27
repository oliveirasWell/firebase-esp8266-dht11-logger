import React from 'react';
import '../css/App.css';
import {Card, CardText, CardHeader} from 'material-ui';
import ReactFireMixin from "reactfire";
import {database} from "../utils/auth.js";
import {Table, TableBody, TableHeader, TableHeaderColumn, TableRow, TableRowColumn} from 'material-ui/Table';

const ListLeitura = React.createClass({
    mixins: [ReactFireMixin],
    getInitialState() {
        return {
            data: []
        };
    },
    componentWillMount() {
        this.bindAsArray(database.ref('leitura').limitToLast(10) /*.orderByChild('data')*/, 'data');
    },
    render(){
        const leituras = this.state.data.map((leitura, index) => {
            return (
                <TableRow key={index}>
                    <TableRowColumn style={{textAlign: 'center'}}>{leitura.data ? leitura.data : '-'}</TableRowColumn>
                    <TableRowColumn style={{textAlign: 'center'}}>{leitura.temperatura}</TableRowColumn>
                    <TableRowColumn style={{textAlign: 'center'}}>{leitura.umidade}</TableRowColumn>
                    <TableRowColumn style={{textAlign: 'center'}}>{leitura.cliente}</TableRowColumn>
                </TableRow>
            );
        });

        return (
            <Card>
                <CardHeader
                    title="Tabela de Leituras"
                    actAsExpander={false}
                    showExpandableButton={false}
                />
                <CardText>
                    <Table multiSelectable={false} selectable={false}>
                        <TableHeader
                            displaySelectAll={false}
                        >
                            <TableRow>
                                <TableHeaderColumn style={{textAlign: 'center'}}><b>Data</b></TableHeaderColumn>
                                <TableHeaderColumn style={{textAlign: 'center'}}><b>Temperatura
                                    (Cº)</b></TableHeaderColumn>
                                <TableHeaderColumn style={{textAlign: 'center'}}><b>Umidade (%)</b></TableHeaderColumn>
                                <TableHeaderColumn style={{textAlign: 'center'}}><b>Cliente</b></TableHeaderColumn>
                            </TableRow>
                        </TableHeader>
                        < TableBody >
                            {leituras}
                        </TableBody >
                    </Table>
                </CardText>
            </Card>
        );
    }
});

export default ListLeitura;