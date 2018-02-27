type state = {
  todos: list(TodoItem.todo),
  input: string,
  formError: bool,
  filter: Filters.t
};

type action =
  | RemoveTodo(TodoItem.todoId)
  | AddTodo(TodoItem.todo)
  | SetFilter(Filters.t)
  | UpdateTodoCompletion(TodoItem.todoId);

open LocalStorage;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    todos: LocalStorage.getAll(),
    input: "",
    formError: false,
    filter: All
  },
  reducer: (action, state) =>
    switch action {
    | AddTodo(todo) =>
      let todos = [todo, ...state.todos];
      LocalStorage.save(todos);
      ReasonReact.Update({...state, input: "", todos});
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
      let todos =
        List.filter((todo: TodoItem.todo) => todo.id != id, state.todos);
      LocalStorage.save(todos);
      ReasonReact.Update({...state, todos});
    | SetFilter(filter) => ReasonReact.Update({...state, filter})
    },
  render: self =>
    <div className="App">
      <Header />
      <Form
        onSubmit=(
          value =>
            self.send(
              AddTodo({
                id: int_of_float(Js.Date.now()),
                value,
                completed: false
              })
            )
        )
      />
      <Filters
        filter=self.state.filter
        onFilterClick=((filter: Filters.t) => self.send(SetFilter(filter)))
      />
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
                List.filter(
                  (todo: TodoItem.todo) =>
                    switch self.state.filter {
                    | All => true
                    | Completed => todo.completed
                    | Remaining => ! todo.completed
                    },
                  self.state.todos
                )
              )
            )
          )
        )
      </div>
    </div>
};