import React from 'react';
import '../css/App.css';
import {Table, TableBody, TableHeader, TableHeaderColumn, TableRow, TableRowColumn} from 'material-ui/Table';

const ListLeitura = React.createClass({
    render(){
        const leituras = this.props.data.map((leitura, index) => {
            return (
                <TableRow key={index} >
                    <TableRowColumn style={{textAlign: 'center'}} >{leitura.data ? leitura.data : '-'}</TableRowColumn>
                    <TableRowColumn style={{textAlign: 'center'}}>{leitura.temperatura}</TableRowColumn>
                    <TableRowColumn style={{textAlign: 'center'}}>{leitura.umidade}</TableRowColumn>
                    <TableRowColumn style={{textAlign: 'center'}}>{leitura.cliente}</TableRowColumn>
                </TableRow>
            );
        });

        return (
            <Table>
                <TableHeader>
                    <TableRow>
                        <TableHeaderColumn style={{textAlign: 'center'}}><b>Data</b></TableHeaderColumn>
                        <TableHeaderColumn style={{textAlign: 'center'}}><b>Temperatura (Cº)</b></TableHeaderColumn>
                        <TableHeaderColumn style={{textAlign: 'center'}}><b>Umidade (%)</b></TableHeaderColumn>
                        <TableHeaderColumn style={{textAlign: 'center'}}><b>Cliente</b></TableHeaderColumn>
                    </TableRow>
                </TableHeader>
                < TableBody >
                    {leituras}
                </TableBody >
            </Table>
        );
    }
});

export default ListLeitura;