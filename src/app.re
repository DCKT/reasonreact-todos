type state = {todos: list(string)};

type action =
  | AddTodo(string);

[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {todos: []},
  reducer: (action, state) =>
    switch action {
    | AddTodo(value) => ReasonReact.Update({todos: [value, ...state.todos]})
    },
  render: self =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement("TODO LIST")) </h2>
        <input _type="text" placeholder="Add a todo" className="App-input" />
        <button onClick=(_event => self.send(AddTodo("test")))>
          (ReasonReact.stringToElement("Add"))
        </button>
      </div>
      <p className="App-intro">
        (
          ReasonReact.arrayToElement(
            Array.of_list(
              List.map(
                todo => <div> (ReasonReact.stringToElement(todo)) </div>,
                self.state.todos
              )
            )
          )
        )
      </p>
    </div>
};