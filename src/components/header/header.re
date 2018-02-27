let component = ReasonReact.statelessComponent("Header");

[@bs.module] external logo : string = "./assets/logo.svg";

[%bs.raw {| require('./header.css')|}];

let make = _children => {
  ...component,
  render: _self =>
    <div className="App-header">
      <img src=logo className="App-logo" alt="logo" />
      <h2> (ReasonReact.stringToElement("TODO LIST")) </h2>
    </div>
};