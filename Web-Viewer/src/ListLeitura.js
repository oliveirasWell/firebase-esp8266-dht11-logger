import React from 'react';
import {Table, TableBody, TableHeader, TableHeaderColumn, TableRow, TableRowColumn} from 'material-ui/Table';

const ListLeitura = React.createClass({
    render(){
        const leituras = this.props.data.map( (leitura, index) => {
            return (
                <TableRow key={index} className='list-leituras' >
                    <TableRowColumn>{leitura.data}</TableRowColumn>
                    <TableRowColumn>{leitura.temperatura}</TableRowColumn>
                    <TableRowColumn>{leitura.umidade}</TableRowColumn>
                    <TableRowColumn>{leitura.cliente}</TableRowColumn>
                </TableRow>
            );
        });

        return (
            <Table>
                <TableHeader>
                    <TableRow>
                        <TableHeaderColumn><b>Data</b></TableHeaderColumn>
                        <TableHeaderColumn><b>Temperatura (Cº)</b></TableHeaderColumn>
                        <TableHeaderColumn><b>Umidade (%)</b></TableHeaderColumn>
                        <TableHeaderColumn><b>Cliente</b></TableHeaderColumn>
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