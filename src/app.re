type filter =
  | All
  | Completed
  | Remaining;

type state = {
  todos: list(TodoItem.todo),
  input: string,
  formError: bool,
  filter
};

type action =
  | RaiseFormError
  | UpdateInput(string)
  | RemoveTodo(TodoItem.todoId)
  | AddTodo(TodoItem.todo)
  | SetFilter(filter)
  | UpdateTodoCompletion(TodoItem.todoId);

let setClassName = (~default="", ~optionals=?, ()) =>
  default
  ++ " "
  ++ String.concat(
       " ",
       switch optionals {
       | None => []
       | Some(list) => list
       }
     );

[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let component = ReasonReact.reducerComponent("App");

[@bs.val] external dateNow : unit => int = "Date.now";

let make = _children => {
  ...component,
  initialState: () => {todos: [], input: "", formError: false, filter: All},
  reducer: (action, state) =>
    switch action {
    | RaiseFormError => ReasonReact.Update({...state, formError: true})
    | AddTodo(value) =>
      ReasonReact.Update({...state, input: "", todos: [value, ...state.todos]})
    | UpdateInput(input) => ReasonReact.Update({...state, input})
    | UpdateTodoCompletion(id) =>
      ReasonReact.Update({
        ...state,
        todos:
          List.map(
            (todo: TodoItem.todo) => {
              ...todo,
              completed: id == todo.id ? ! todo.completed : todo.completed
            },
            state.todos
          )
      })
    | RemoveTodo(id) =>
      ReasonReact.Update({
        ...state,
        todos: List.filter((todo: TodoItem.todo) => todo.id != id, state.todos)
      })
    | SetFilter(filter) => ReasonReact.Update({...state, filter})
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
              switch self.state.input {
              | "" => self.send(RaiseFormError)
              | _ =>
                self.send(
                  AddTodo({
                    id: dateNow(),
                    completed: false,
                    value: self.state.input
                  })
                )
              };
            }
          )>
          <div>
            <input
              _type="text"
              placeholder="Add a todo"
              className="App-input"
              value=self.state.input
              onChange=(
                event =>
                  self.send(
                    UpdateInput(
                      ReactDOMRe.domElementToObj(
                        ReactEventRe.Form.target(event)
                      )##value
                    )
                  )
              )
            />
            <button className="App-input-submit">
              (ReasonReact.stringToElement("Add"))
            </button>
          </div>
          (
            self.state.formError ?
              <p className="App-error">
                (ReasonReact.stringToElement("Field required"))
              </p> :
              ReasonReact.nullElement
          )
        </form>
        <div className="App-filters">
          <button
            className=(
              setClassName(
                ~default="App-filter",
                ~optionals=[
                  self.state.filter == All ? "App-filter--active" : ""
                ],
                ()
              )
            )
            onClick=(_event => self.send(SetFilter(All)))>
            (ReasonReact.stringToElement("All"))
          </button>
          <button
            className=(
              setClassName(
                ~default="App-filter",
                ~optionals=[
                  self.state.filter == Completed ? "App-filter--active" : ""
                ],
                ()
              )
            )
            onClick=(_event => self.send(SetFilter(Completed)))>
            (ReasonReact.stringToElement("Completed"))
          </button>
          <button
            className=(
              setClassName(
                ~default="App-filter",
                ~optionals=[
                  self.state.filter == Remaining ? "App-filter--active" : ""
                ],
                ()
              )
            )
            onClick=(_event => self.send(SetFilter(Remaining)))>
            (ReasonReact.stringToElement("Remaining"))
          </button>
        </div>
      </div>
      <div className="App-intro">
        (
          ReasonReact.arrayToElement(
            Array.of_list(
              List.map(
                (todo: TodoItem.todo) =>
                  <TodoItem
                    key=(string_of_int(todo.id))
                    todo
                    onCompletionChange=(
                      _event => self.send(UpdateTodoCompletion(todo.id))
                    )
                    onDelete=(_event => self.send(RemoveTodo(todo.id)))
                  />,
                switch self.state.filter {
                | All => self.state.todos
                | Completed =>
                  List.filter(
                    (todo: TodoItem.todo) => todo.completed,
                    self.state.todos
                  )
                | Remaining =>
                  List.filter(
                    (todo: TodoItem.todo) => ! todo.completed,
                    self.state.todos
                  )
                }
              )
            )
          )
        )
      </div>
    </div>
};