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

let component = ReasonReact.reducerComponent("App");

[@bs.val] external dateNow : unit => int = "Date.now";

let make = _children => {
  ...component,
  initialState: () => {todos: [], input: "", formError: false, filter: All},
  reducer: (action, state) =>
    switch action {
    | AddTodo(value) =>
      ReasonReact.Update({...state, input: "", todos: [value, ...state.todos]})
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
      <Header />
      <Form
        onSubmit=(
          value => self.send(AddTodo({id: dateNow(), value, completed: false}))
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