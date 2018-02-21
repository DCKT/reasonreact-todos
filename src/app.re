type state = {
  todos: list(string),
  input: string
};

type action =
  | UpdateInput(string)
  | AddTodo(string);

[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {todos: [], input: ""},
  reducer: (action, state) =>
    switch action {
    | AddTodo(value) =>
      ReasonReact.Update({...state, todos: [value, ...state.todos]})
    | UpdateInput(input) => ReasonReact.Update({...state, input})
    },
  render: self =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement("TODO LIST")) </h2>
        <form
          onSubmit=(
            event => {
              ReactEventRe.Form.preventDefault(event);
              self.send(AddTodo(self.state.input));
            }
          )>
          <input
            _type="text"
            placeholder="Add a todo"
            className="App-input"
            onChange=(
              event =>
                self.send(
                  UpdateInput(
                    ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value
                  )
                )
            )
          />
          <button> (ReasonReact.stringToElement("Add")) </button>
        </form>
      </div>
      <div className="App-intro">
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
      </div>
    </div>
};